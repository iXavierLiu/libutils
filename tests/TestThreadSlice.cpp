#include "gtest/gtest.h"
#include "libutils/ThreadSlice.h"
#include "libutils/Time.h"

#include <algorithm>
#include <thread>
#include <unordered_set>
#include <vector>

using namespace libutils;

struct ThreadContex
{
	std::mutex mtx;
	uint64_t base = 100000;
	const uint64_t range = 100;
	// std::map<std::thread::id, std::unordered_set<uint64_t>> data;
	std::map<std::thread::id, std::vector<uint64_t>> data;
};

bool isPrimary(uint64_t number)
{
	if (number < 2) return false;

	for (uint32_t i = 2; i * i <= number; ++i)
	{
		if (number % i == 0) return false;
	};
	return true;
}

TEST(TestThreadSlice, Primary)
{
	const uint8_t threadNumber = 10;
	auto ctxPtr = std::make_shared<ThreadContex>();

	// 启动多线程计算素数，每个线程每次计算 [base,base+range) 区间的素数，并将结果放入公共上下文中
	auto threadSlicePtr = ThreadSliceProxy<ThreadContex>::create(
		[](ThreadSliceProxy<ThreadContex>::SharedContex ctx) -> bool {
			const std::thread::id tid = std::this_thread::get_id();
			uint64_t base = 0;
			{
				std::lock_guard<std::mutex> lck(ctx->mtx);
				if (ctx->data.end() == ctx->data.find(tid)) ctx->data.insert({ tid, {} });
				base = ctx->base;
				ctx->base += ctx->range;
			}

			uint64_t stop = base + ctx->range;
			for (uint64_t i = base; i < stop; ++i)
			{
				if (!isPrimary(i)) continue;

				{
					std::lock_guard<std::mutex> lck(ctx->mtx);
					ctx->data[tid].push_back(i);
					// ctx->data[tid].insert(i);
				}
			}
			return true;
		},
		ctxPtr.get());

	// 实例默认有一个线程，添加剩余线程
	threadSlicePtr->Add(threadNumber - 1);

	std::this_thread::sleep_for(std::chrono::seconds(10));

	// 停止所有线程并计算停止所用时间
	auto ts = Time::steady_time();
	threadSlicePtr.reset();
	auto timeUse = Time::timing_ms(ts);

	EXPECT_LE(timeUse, 50);

	for (int i = 0; i < 10; ++i)
	{
		ASSERT_TRUE(ctxPtr->mtx.try_lock());  // no more thread use
		ctxPtr->mtx.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	EXPECT_LE(timeUse, 1000ull);  // expect time use less than 1000ms

	ASSERT_EQ(ctxPtr->data.size(), threadNumber);

	std::for_each(ctxPtr->data.begin(), ctxPtr->data.end(), [](const auto& item) {
		// expect primary count more than 10000 in each thread
		ASSERT_GE(item.second.size(), 10000);
		GTEST_LOG_(INFO) << "tid: " << item.first << ", calculated primary number count: " << item.second.size();
	});
}