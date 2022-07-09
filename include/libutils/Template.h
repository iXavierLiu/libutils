#pragma once
#include <type_traits>

class Template
{
public:
	template <size_t begin, size_t end>
	static auto __make_index_range();

	template <size_t base, size_t... indexes>
	static auto __make_index_base(std::index_sequence<indexes...>);
};

template <size_t begin, size_t end>
inline auto Template::__make_index_range()
{
	return __make_index_base<begin>(std::make_index_sequence<end - begin>());
}

template <size_t base, size_t... indexes>
inline auto Template::__make_index_base(std::index_sequence<indexes...>)
{
	return std::index_sequence<base + indexes...>{};
}
