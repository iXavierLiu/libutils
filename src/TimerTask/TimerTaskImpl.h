#pragma once
#include "../Daemon/DaemonImpl.h"
#include "libutils/TimerTaskContext.h"

#include <functional>
#include <set>

namespace libutils {

class TimerTaskImpl
{
public:
	TimerTaskImpl(std::function<bool()> func, uint64_t intervalMilSec, uint64_t times, bool immediately);

	virtual ~TimerTaskImpl() = default;

	bool TimeSlice(const std::chrono::steady_clock::time_point& time);

public:
	TaskContext taskCtx;
};

}  // namespace libutils