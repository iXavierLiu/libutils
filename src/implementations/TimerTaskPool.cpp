#include "libutils/TimerTaskPool.h"

#include "TimerTask/TimerTaskPoolImpl.h"

using namespace libutils;

TimerTaskPool::~TimerTaskPool()
{
	if (impl) delete impl;
}

TimerTaskPool::Ptr TimerTaskPool::create()
{
	return Ptr(new TimerTaskPool());
}

bool libutils::TimerTaskPool::Add(TimerTask::Ptr ptr)
{
	return impl->Add(ptr);
}

bool libutils::TimerTaskPool::Delete(TimerTask::Ptr ptr)
{
	return impl->Delete(ptr);
}

libutils::TimerTaskPool::TimerTaskPool() : impl(new TimerTaskPoolImpl()) {}
