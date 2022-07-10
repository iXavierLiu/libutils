#include "gtest/gtest.h"
#include "libutils/ThreadSlice.h"
#include "libutils/Time.h"

#include <algorithm>
#include <thread>
#include <unordered_set>
#include <vector>

using namespace libutils;

class ThreadSlicePrimaryTest : public ::testing::Test
{
protected:
	struct ThreadContext;

protected:
	virtual void SetUp() override
	{
		ctxPtr = std::make_shared<ThreadContext>();
		ptr = ThreadSlice::create(&ThreadSlicePrimaryTest::ThreadJod, this, ctxPtr, std::this_thread::get_id());
		ASSERT_NE(ptr.get(), nullptr);
	}
	virtual void TearDown() override
	{
		ptr.reset();
		ctxPtr.reset();
	}

	static void SetUpTestSuite() {}
	static void TearDownTestSuite() {}

private:
	static bool isPrime(uint64_t number)
	{
		if (number < 2) return false;

		for (uint32_t i = 2; i * i <= number; ++i)
		{
			if (number % i == 0) return false;
		};
		return true;
	}

	bool ThreadJod(std::shared_ptr<ThreadContext> ctx, std::thread::id parentThreadId)
	{
		const std::thread::id tid = std::this_thread::get_id();
		uint64_t rangeStart = 0;
		uint64_t rangeStop = 0;

		{
			std::lock_guard<std::mutex> lck(ctx->mtx);

			// 添加本线程对应的上下文
			if (ctx->data.end() == ctx->data.find(tid)) ctx->data.insert({ tid, {} });

			// 获取要计算的范围并更新共享上下文
			rangeStart = ctx->base;
			ctx->base += ctx->range;
			rangeStop = ctx->base;
		}

		// 计算 [rangeStart, rangeStop) 区间的素数
		for (uint64_t number = rangeStart; number < rangeStop; ++number)
		{
			if (!isPrime(number)) continue;

			{
				std::lock_guard<std::mutex> lck(ctx->mtx);
				ctx->data[tid].push_back(number);
			}
		}
		return true;
	}

protected:
	ThreadSlice::Ptr ptr;

	struct ThreadContext
	{
		std::mutex mtx;
		uint64_t base = 100000;
		const uint64_t range = 100;
		std::map<std::thread::id, std::vector<uint64_t>> data;
	};

	std::shared_ptr<ThreadContext> ctxPtr;
};

TEST_F(ThreadSlicePrimaryTest, EmptySizeTest)
{
	ASSERT_EQ(ptr->GetCount(), 0);

	std::lock_guard<std::mutex> lck(ctxPtr->mtx);
	ASSERT_EQ(ctxPtr->data.size(), 0);
}

TEST_F(ThreadSlicePrimaryTest, MutiThreadCalcPrimeTest)
{
	const uint8_t threadNumber = 10;

	ptr->Add(threadNumber);
	ASSERT_EQ(ptr->GetCount(), threadNumber);

	std::this_thread::sleep_for(std::chrono::seconds(10));

	// 停止所有线程并计算停止所用时间
	auto ts = Time::steady_time();
	ptr.reset();
	auto timeUse = Time::timing_ms(ts);

	EXPECT_LE(timeUse, 50);	 // expect time use less than 50 ms

	// test no more thread use
	for (int i = 0; i < 10; ++i)
	{
		ASSERT_TRUE(ctxPtr->mtx.try_lock());
		ctxPtr->mtx.unlock();
		std::this_thread::yield();
	}

	ASSERT_EQ(ctxPtr->data.size(), threadNumber);

	std::for_each(ctxPtr->data.begin(), ctxPtr->data.end(), [](const auto& item) {
		// expect primary count more than 10000 in each thread
		ASSERT_GE(item.second.size(), 10000);
		GTEST_LOG_(INFO) << "tid: " << item.first << ", calculated primary number count: " << item.second.size();
	});
}

TEST(ThreadSlice, LambdaFunctionTest)
{
	uint32_t sum = 0;
	uint32_t counter = 0;
	uint32_t end = 1000;
	auto ptr = ThreadSlice::create(
		[](uint32_t& s, uint32_t& i, const uint32_t e) -> bool {
			s += i;
			if (++i > e) return false;
			return true;
		},
		sum, counter, end);
	ASSERT_NE(ptr.get(), nullptr);

	ptr->Add(1);
	ASSERT_EQ(ptr->GetCount(), 1);

	std::this_thread::sleep_for(std::chrono::seconds(2));
	ASSERT_EQ(ptr->GetCount(), 0);

	ptr.reset();
	ASSERT_EQ(sum, (1 + end) * end / 2);
}

bool StaticFunctionTestFunc(const uint32_t stop)
{
	static uint32_t i = 0;
	if (i > stop) return false;
	++i;
	return true;
}

TEST(ThreadSlice, StaticFunctionTest)
{
	auto ptr = ThreadSlice::create(StaticFunctionTestFunc, 1000);
	ASSERT_NE(ptr.get(), nullptr);

	ptr->Add(1);
	ASSERT_EQ(ptr->GetCount(), 1);

	std::this_thread::sleep_for(std::chrono::seconds(2));
	ASSERT_EQ(ptr->GetCount(), 0);
}