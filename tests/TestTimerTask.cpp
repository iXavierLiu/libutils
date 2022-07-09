#include "gtest/gtest.h"
#include "libutils/Time.h"
#include "libutils/TimerTask.h"
#include "libutils/TimerTaskPool.h"

using namespace libutils;

TEST(TestTimerTask, Timer)
{
	ASSERT_TRUE(true);
	uint8_t times1 = 0;
	uint8_t times2 = 0;
	uint32_t interval1 = 1000;
	uint32_t interval2 = 500;

	// ASSERT_* 有无返回值，在回调中使用回发生返回值类型不一致的问题，这里用lambda封装
	auto lt = [](uint64_t a, uint64_t b) { ASSERT_LE(a, b); };

	auto poolPtr = TimerTaskPool::create();
	poolPtr->Add(TimerTask::create(
		[&]() -> bool {
			static auto last = Time::steady_time();
			if (times1++ == 0) return true;

			lt(abs(Time::timing_ms(last) - interval1), interval1 / 20);	 // 计算实际用时与interval的差
			last = Time::steady_time();
			return true;
		},
		interval1, 5, true));

	poolPtr->Add(TimerTask::create(
		[&]() -> bool {
			static auto last = Time::steady_time();
			if (times2++ == 0) return true;

			lt(abs(Time::timing_ms(last) - interval2), interval2 / 20);	 // 计算实际用时与interval的差
			last = Time::steady_time();
			return true;
		},
		interval2, 0, true));

	std::this_thread::sleep_for(std::chrono::seconds(10));

	// 停止所有线程并计算停止所用时间
	auto ts = Time::steady_time();
	poolPtr.reset();
	auto timeUse = Time::timing_ms(ts);

	EXPECT_LE(timeUse, 50);

	ASSERT_EQ(times1, 5);

	ASSERT_GE(times2, 18);
	ASSERT_LE(times2, 22);
}