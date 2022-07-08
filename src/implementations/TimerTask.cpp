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

TaskContext* libutils::TimerTask::GetContext()
{
	return &impl->taskCtx;
}
