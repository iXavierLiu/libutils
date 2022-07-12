#include "libutils/Time.h"

#include <chrono>

using namespace libutils;

std::chrono::system_clock::time_point Time::sys_time()
{
	return std::chrono::system_clock::now();
}

std::chrono::steady_clock::time_point Time::steady_time()
{
	return std::chrono::steady_clock::now();
}

int64_t Time::sys_ts()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(sys_time().time_since_epoch()).count();
}

int64_t Time::steady_ts()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(steady_time().time_since_epoch()).count();
}

int64_t Time::timing_ms(std::chrono::system_clock::time_point tm)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(Time::sys_time() - tm).count();
}

int64_t Time::timing_ms(std::chrono::steady_clock::time_point tm)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(Time::steady_time() - tm).count();
}