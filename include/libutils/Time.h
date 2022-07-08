#pragma once

#include <chrono>
#include <type_traits>

namespace libutils {

class Time
{
public:
	// 时间
	static std::chrono::system_clock::time_point sys_time();
	static std::chrono::steady_clock::time_point steady_time();

	// 时间戳
	static int64_t sys_ts();
	static int64_t steady_ts();

	// 时间差
	static int64_t timing_ms(std::chrono::system_clock::time_point tm);
	static int64_t timing_ms(std::chrono::steady_clock::time_point tm);
};

}  // namespace libutils