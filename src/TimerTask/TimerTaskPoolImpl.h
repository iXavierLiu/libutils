#pragma once

#include "libutils/Daemon.h"
#include "libutils/TimerTask.h"

#include <memory>
#include <mutex>
#include <set>

namespace libutils {

class TimerTaskPoolImpl : protected Daemon, protected IDaemon
{
protected:
	// 通过 Daemon 继承
	virtual void Job();

public:
	TimerTaskPoolImpl();
	virtual ~TimerTaskPoolImpl() override;

	bool Add(TimerTask::Ptr ptr);
	bool Delete(TimerTask::Ptr ptr);

private:
	std::set<TimerTask::Ptr> timers;
	std::mutex timersMtx;
	const uint32_t expectedAvgMilSecPerRound = 10;
};

}  // namespace libutils