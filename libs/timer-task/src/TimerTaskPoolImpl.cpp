#include "TimerTaskPoolImpl.h"

#include "libutils/Time.h"

#ifdef _WIN32
#include <Windows.h>
#pragma comment(lib, "Winmm.lib")
#endif	// _WIN32

using namespace libutils;

void TimerTaskPoolImpl::Job()
{
	while (Daemon::IsRunnable())
	{
		std::unique_lock<std::mutex> lck(timersMtx);

		auto time = Time::steady_time();

		for (auto it = timers.begin(); it != timers.end() && Daemon::IsRunnable();)
		{
			if ((*it)->TimeSlice(time))
			{
				it = timers.erase(it);
				continue;
			}
			++it;
		};
		lck.unlock();

		auto timeUseMilSec = Time::timing_ms(time);
		auto sleepMilSec = timeUseMilSec >= expectedAvgMilSecPerRound ? 0 : expectedAvgMilSecPerRound - timeUseMilSec;

#if _DEBUG
		if (!sleepMilSec) printf("[TimerTaskPoolImpl] out of time, round time use: %lld ms\n", timeUseMilSec);
#endif
#ifdef _WIN32
		// setting minimum timer resolution
		timeBeginPeriod(1);
#endif	// _WIN32
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepMilSec));
#ifdef _WIN32
		timeEndPeriod(1);
#endif	// _WIN32
	};
}

TimerTaskPoolImpl::~TimerTaskPoolImpl()
{
	// 这里需要先销毁子进程
	Daemon::Stop();
}

bool TimerTaskPoolImpl::Add(TimerTask::Ptr ptr)
{
	std::lock_guard<std::mutex> lck(timersMtx);
	if (timers.end() != timers.find(ptr)) return false;

	timers.insert(ptr);
	return true;
}

bool TimerTaskPoolImpl::Delete(TimerTask::Ptr ptr)
{
	std::lock_guard<std::mutex> lck(timersMtx);
	if (timers.end() == timers.find(ptr)) return false;

	timers.erase(ptr);
	return true;
}

TimerTaskPoolImpl::TimerTaskPoolImpl() : Daemon(this)
{
	Daemon::Start();
}
