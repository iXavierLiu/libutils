#include "DaemonImpl.h"

#include <cassert>
using namespace libutils;

DaemonImpl::DaemonImpl(IDaemon* idaemonPtr) : idaemonPtr(idaemonPtr)
{
	assert(idaemonPtr);
};

DaemonImpl::~DaemonImpl()
{
	Stop(true);
}

bool DaemonImpl::Start()
{
	std::lock_guard<std::mutex> lck(mtx);
	if (isRunning) return false;

	isRunnable = true;
	isRunning = true;

	thread = std::thread(&DaemonImpl::JobWrapper, this);
	return true;
}

bool DaemonImpl::Stop(bool sync)
{
	{
		std::lock_guard<std::mutex> lck(mtx);
		if (!isRunning && !thread.joinable()) return false;
		isRunnable = false;
	}

	if (sync)
	{
		if (thread.joinable()) { thread.join(); }
	}

	return true;
}

bool DaemonImpl::IsRunnable()
{
	return isRunnable;
}

void DaemonImpl::JobWrapper()
{
	idaemonPtr->Job();
	
	std::lock_guard<std::mutex> lck(mtx);
	isRunning = false;
}
