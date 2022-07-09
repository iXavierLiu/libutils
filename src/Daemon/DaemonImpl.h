#pragma once

#include "libutils/IDaemon.h"

#include <mutex>
#include <thread>

namespace libutils {

class DaemonImpl
{
public:
	DaemonImpl(IDaemon* idaemonPtr);

	virtual ~DaemonImpl();

	bool Start();

	bool Stop(bool sync);

	bool IsRunnable();

private:
	void JobWrapper();

private:
	std::thread thread;
	bool isRunnable = false;
	bool isRunning = false;
	std::mutex mtx;
	IDaemon* idaemonPtr;
};

};	// namespace libutils
