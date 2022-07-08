#pragma once

#include "TimerTaskContext.h"

namespace std {
template <class T>
class function;

template <class _Ty>
class shared_ptr;
}  // namespace std

namespace libutils {

class TimerTaskImpl;
class TimerTask
{
public:
	using Ptr = std::shared_ptr<TimerTask>;

	friend class TimerTaskPoolImpl;

public:
	virtual ~TimerTask();

	static Ptr create(std::function<bool()> func, uint64_t intervalMilSec, uint64_t times = 1, bool immediately = false);

	TaskContext* GetContext();

private:
	TimerTask(std::function<bool()> func, uint64_t intervalMilSec, uint64_t times, bool immediately);

private:
	TimerTaskImpl* impl;
};

}  // namespace libutils