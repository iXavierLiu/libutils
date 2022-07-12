#pragma once

#include "Template.h"

#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>

class TemplateProxy
{
public:
	using PackType = std::shared_ptr<void>;

public:
	/// <summary>
	/// 将参数打包成 PackType 类型的变量
	/// </summary>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="...args"></param>
	/// <returns></returns>
	template <typename... Args>
	static PackType Pack(Args&&... args);

	/// <summary>
	/// 从 PackType 类型的变量解包
	/// </summary>
	/// <typeparam name="F"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="pack"></param>
	/// <param name="...args"></param>
	template <typename... Args>
	static std::shared_ptr<std::tuple<Args...>> Unpack(PackType pack);

	/// <summary>
	/// 通过解包出来的参数调用指定函数
	/// </summary>
	/// <typeparam name="F"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="f"></param>
	/// <param name="tuple"></param>
	/// <returns></returns>
	template <typename F, typename... Args>
	static auto apply(F&& f, std::shared_ptr<std::tuple<Args...>> tuple);

private:
	template <size_t... indexes, typename F, typename... Args>
	static auto __apply(std::index_sequence<indexes...>, F&& f, std::shared_ptr<std::tuple<Args...>> tuple);
};

template <typename... Args>
inline TemplateProxy::PackType TemplateProxy::Pack(Args&&... args)
{
	return std::make_shared<std::tuple<Args...>>(std::forward_as_tuple(std::forward<Args>(args)...));
}

template <typename... Args>
inline std::shared_ptr<std::tuple<Args...>> TemplateProxy::Unpack(PackType pack)
{
	return std::static_pointer_cast<std::tuple<Args...>>(pack);
}

template <typename F, typename... Args>
inline auto TemplateProxy::apply(F&& f, std::shared_ptr<std::tuple<Args...>> tuple)
{
	return __apply(Template::__make_index_range<0, sizeof...(Args)>(), std::forward<F>(f), std::forward<std::shared_ptr<std::tuple<Args...>>>(tuple));
}

template <size_t... indexes, typename F, typename... Args>
inline auto TemplateProxy::__apply(std::index_sequence<indexes...>, F&& f, std::shared_ptr<std::tuple<Args...>> tuple)
{
	return std::invoke(std::forward<F>(f), std::get<indexes>(*tuple.get())...);
}
