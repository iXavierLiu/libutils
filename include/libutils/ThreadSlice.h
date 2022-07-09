#pragma once
#include "TemplateProxy.h"

#include <functional>
#include <memory>

namespace libutils {

class ThreadSliceImpl;

/// <summary>
/// 多线程类的代理类
///
/// 由于【模板类】无法将定义和实现的分离，所以ThreadSlice通过将【不定参数】打包成一个【参数包】交由ThreadSliceProxy代理类将包透传给回调函数，
/// 由ThreadSlice解包并实现最终回调
/// </summary>
class ThreadSliceProxy
{
public:
	using Ptr = std::shared_ptr<ThreadSliceProxy>;
	using ArgsPackType = std::shared_ptr<void>;
	using FuncType = std::function<bool(ArgsPackType)>;

public:
	virtual ~ThreadSliceProxy();

	static Ptr create(FuncType func, ArgsPackType argsPack);

	bool Add(uint8_t count);

private:
	ThreadSliceProxy(FuncType func, ArgsPackType argsPack);

private:
	ThreadSliceImpl* impl;
};

/// <summary>
/// 多线程类
///
/// 以指定的回调参数重复地调用指定的回调函数
/// </summary>
class ThreadSlice : protected ThreadSliceProxy
{
public:
	/// <summary>
	/// 创建一个多线程类并返回代理类的实例
	///
	/// 示例：
	///     create([](int a, int b)->bool{ ...; return ToBeContinue(); }, variableOrExprA, variableOrExprB);
	/// </summary>
	/// <typeparam name="F">回调函数类型</typeparam>
	/// <typeparam name="...Args">回调参数类型</typeparam>
	/// <param name="func">形如 [](Args... args)->bool{} 的回调函数</param>
	/// <param name="...args">调用回调函数时给出的参数，如果参数是变量或指针，调用者需要维护它的生命周期</param>
	/// <returns>多线程代理类的实例</returns>
	template <typename F, typename... Args>
	static Ptr create(F&& func, Args&&... args)
	{
		return ThreadSliceProxy::create(
			[=](ThreadSliceProxy::ArgsPackType ctx) {
				// 解包参数后调用回调
				return TemplateProxy::apply(func, TemplateProxy::Unpack<Args...>(ctx));
			},
			TemplateProxy::Pack(std::forward<Args>(args)...));
	}
};

};	// namespace libutils