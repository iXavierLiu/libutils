#include "TimerTaskImpl.h"

#include "libutils/Time.h"

#include <algorithm>
#include <memory>

using namespace libutils;

TimerTaskImpl::TimerTaskImpl(std::function<bool()> func, uint64_t intervalMilSec, uint64_t times, bool immediately)
{
	taskCtx.func = func;
	taskCtx.intervalMilSec = std::chrono::milliseconds(intervalMilSec);
	taskCtx.nextTime = Time::steady_time() + (immediately ? std::chrono::milliseconds(0) : taskCtx.intervalMilSec);
	taskCtx.times = times;
	taskCtx.always = times == 0;
}

bool TimerTaskImpl::TimeSlice(const std::chrono::steady_clock::time_point& time)
{
	// 未到执行时间
	if (taskCtx.nextTime > time) return false;

	taskCtx.func();

	// 次数执行完毕
	if (!taskCtx.always && --taskCtx.times == 0) return true;

	taskCtx.nextTime = time + taskCtx.intervalMilSec;
	return false;
}
