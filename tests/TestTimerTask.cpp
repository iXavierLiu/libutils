#include "gtest/gtest.h"
#include "libutils/Time.h"
#include "libutils/TimerTask.h"
#include "libutils/TimerTaskPool.h"

#include <thread>

using namespace libutils;
TEST(TestTimerTask, Immediately)
{
	auto poolPtr = TimerTaskPool::create();

	int64_t tsNow = Time::steady_ts();
	int64_t tsImmediately = 0;
	int64_t tsNotImmediately = 0;

	poolPtr->Add(TimerTask::create(
		[&]() -> bool {
			tsImmediately = Time::steady_ts();
			return true;
		},
		100, 1, true));
	poolPtr->Add(TimerTask::create(
		[&]() -> bool {
			tsNotImmediately = Time::steady_ts();
			return true;
		},
		100, 1, false));

	std::this_thread::sleep_for(std::chrono::milliseconds(150));
	poolPtr.reset();

	ASSERT_NEAR(tsImmediately, tsNow, 10);
	ASSERT_NEAR(tsNotImmediately, tsNow + 100, 10);
}

TEST(TestTimerTask, Times)
{
	auto poolPtr = TimerTaskPool::create();

	size_t times0 = 0;
	size_t times5 = 0;
	poolPtr->Add(TimerTask::create(
		[&]() -> bool {
			++times0;
			return true;
		},
		25, 0, true));
	poolPtr->Add(TimerTask::create(
		[&]() -> bool {
			++times5;
			return true;
		},
		25, 5, true));

	std::this_thread::sleep_for(std::chrono::milliseconds(250));
	poolPtr.reset();

	ASSERT_NEAR(times0, 10, 1);
	ASSERT_NEAR(times5, 5, 0);
}

TEST(TestTimerTask, Interval)
{
	srand(time(nullptr));
	size_t interval = rand() % 40 + 10;
	int64_t ts = 0;

	auto poolPtr = TimerTaskPool::create();
	poolPtr->Add(TimerTask::create(
		[&]() -> bool {
			auto now = Time::steady_ts();
			if (!ts)
			{
				ts = now;
				return true;
			}

			[&]() { ASSERT_NEAR(now, ts, interval + 5); }();

			ts = now;
			return true;
		},
		interval, 0, true));
	std::this_thread::sleep_for(std::chrono::seconds(1));
	poolPtr.reset();
}