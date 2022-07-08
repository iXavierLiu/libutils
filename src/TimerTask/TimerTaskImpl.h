#pragma once

#include <functional>
#include <set>
#include <chrono>

namespace libutils {

class TimerTaskImpl
{
public:
	TimerTaskImpl(std::function<bool()> func, uint64_t intervalMilSec, uint64_t times, bool immediately);

	virtual ~TimerTaskImpl() = default;

	bool TimeSlice(const std::chrono::steady_clock::time_point& time);

private:
	struct
	{
		std::function<bool()> func = nullptr;
		std::chrono::milliseconds intervalMilSec{ 0 };
		std::chrono::steady_clock::time_point nextTime{};
		uint64_t times = 1;
		bool always = false;
	} taskCtx;
};

}  // namespace libutils