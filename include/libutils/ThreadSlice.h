#pragma once
#include <functional>
#include <memory>

namespace libutils {

class ThreadSliceImpl;

/// <summary>
/// 多线程类
///
/// 以指定线程数(默认一个)重复调用回调函数，并在回调参数中给出一个线程间共享的上下文
/// </summary>
class ThreadSlice
{
public:
	using Ptr = std::shared_ptr<ThreadSlice>;
	using SharedContex = void*;

public:
	virtual ~ThreadSlice();
	static Ptr create(std::function<bool(SharedContex)> func, SharedContex ctx);

	bool Add(uint8_t count);

private:
	ThreadSlice(std::function<bool(SharedContex)> func, SharedContex ctx);

private:
	ThreadSliceImpl* impl;
};

/// <summary>
/// 多线程类的代理类
///
/// 由于模板类无法实现定义和实现的分离，实际使用中可通过本代理类以指定的上下文类型创建多线程实例
/// </summary>
/// <typeparam name="T">线程间共享的上下文类型</typeparam>
template <typename T>
class ThreadSliceProxy : public ThreadSlice
{
public:
	using Ptr = ThreadSlice::Ptr;
	using SharedContex = T*;

public:
	static Ptr create(std::function<bool(SharedContex)> func, SharedContex ctx)
	{
		return ThreadSlice::create([=](ThreadSlice::SharedContex __ctx) { return func(static_cast<SharedContex>(__ctx)); }, static_cast<ThreadSlice::SharedContex>(ctx));
	}
};

};	// namespace libutils