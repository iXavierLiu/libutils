#include "libutils/TimerTask.h"

#include "TimerTask/TimerTaskImpl.h"

#include <memory>

using namespace libutils;

TimerTask::TimerTask(std::function<bool()> func, uint64_t intervalMilSec, uint64_t times, bool immediately) : impl(new TimerTaskImpl(func, intervalMilSec, times, immediately)) {}

TimerTask::~TimerTask()
{
	if (impl) delete impl;
}

TimerTask::Ptr TimerTask::create(std::function<bool()> func, uint64_t intervalMilSec, uint64_t times, bool immediately)
{
	return Ptr(new TimerTask(func, intervalMilSec, times, immediately));
}

bool TimerTask::TimeSlice(const std::chrono::steady_clock::time_point& time)
{
	return impl->TimeSlice(time);
}
