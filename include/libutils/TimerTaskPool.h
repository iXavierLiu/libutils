#pragma once

#include "libutils/TimerTask.h"

namespace std {

template <class _Ty>
class shared_ptr;

}  // namespace std

namespace libutils {

class TimerTaskPoolImpl;
class TimerTaskPool
{
	using Ptr = std::shared_ptr<TimerTaskPool>;

public:
	virtual ~TimerTaskPool();
	static Ptr create();

	bool Add(TimerTask::Ptr ptr);
	bool Delete(TimerTask::Ptr ptr);

private:
	TimerTaskPool();

private:
	TimerTaskPoolImpl* impl;
};

}  // namespace libutils