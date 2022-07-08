#include "TimerTaskPoolImpl.h"

using namespace libutils;

void TimerTaskPoolImpl::Job()
{
	while (Daemon::IsRunnable())
	{
		std::unique_lock<std::mutex> lck(timersMtx);

		for (auto it = timers.begin(); it != timers.end();)
		{
			auto now = std::chrono::steady_clock::now();
			auto& ctx = *(*it)->GetContext();

			// 未到执行时间
			if (ctx.nextTime > now)
			{
				++it;
				continue;
			};

			ctx.func();

			// 次数执行完毕
			if (!ctx.always && --ctx.times == 0)
			{
				it = timers.erase(it);
				continue;
			};

			ctx.nextTime = now + ctx.intervalMilSec;
			++it;
		};
		lck.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	};
}

TimerTaskPoolImpl::~TimerTaskPoolImpl()
{
	// 这里需要先销毁子进程
	Daemon::Stop(true);
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
