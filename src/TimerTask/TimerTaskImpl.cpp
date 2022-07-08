#include "TimerTaskImpl.h"

#include <algorithm>
#include <memory>

using namespace libutils;

TimerTaskImpl::TimerTaskImpl(std::function<bool()> func, uint64_t intervalMilSec, uint64_t times, bool immediately)
{
	taskCtx.func = func;
	taskCtx.intervalMilSec = std::chrono::milliseconds(intervalMilSec);
	taskCtx.nextTime = std::chrono::steady_clock::now() + (immediately ? std::chrono::milliseconds(0) : taskCtx.intervalMilSec);
	taskCtx.times = times;
	taskCtx.always = times == 0;
}
