#include "gtest/gtest.h"
#include "libutils/Daemon.h"

#include <mutex>
#include <thread>
using namespace libutils;

std::mutex threadRunningMtx;
class TestDeamon_Common : public Daemon, IDaemon
{
public:
	TestDeamon_Common() : Daemon(this)
	{
		Daemon::Start();
	}

	bool Start()
	{
		return Daemon::Start();
	}

	bool Stop()
	{
		return Daemon::Stop(true);
	}

private:
	virtual void Job() override
	{
		std::lock_guard<std::mutex> lck(threadRunningMtx);
		while (Daemon::IsRunnable())
		{
			std::this_thread::yield();
		}
	}
};

TEST(TestDeamon, Main)
{
	std::shared_ptr<TestDeamon_Common> ptr = std::make_shared<TestDeamon_Common>();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	ASSERT_FALSE(threadRunningMtx.try_lock());
	ptr.reset();
	ASSERT_TRUE(threadRunningMtx.try_lock());
	threadRunningMtx.unlock();
}

TEST(TestDeamon, StartAndStop)
{
	std::shared_ptr<TestDeamon_Common> ptr = std::make_shared<TestDeamon_Common>();
	ASSERT_TRUE(ptr->Stop());
	ASSERT_TRUE(threadRunningMtx.try_lock());
	threadRunningMtx.unlock();
	ASSERT_TRUE(ptr->Start());
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	ASSERT_FALSE(threadRunningMtx.try_lock());
	ptr.reset();
	ASSERT_TRUE(threadRunningMtx.try_lock());
	threadRunningMtx.unlock();
}