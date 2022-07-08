#include "libutils/Time.h"
#include "libutils/TimerTask.h"
#include "libutils/TimerTaskPool.h"

#include <random>
#include <thread>

int main(int argc, char* argv[])
{
	using namespace libutils;

	auto pool = TimerTaskPool::create();

	std::default_random_engine random;
	random.seed(static_cast<uint32_t>(Time::sys_ts()));

	for (int i = 0; i < 20000; ++i)
	{
		pool->Add(TimerTask::create(
			[&random]() -> bool {
				auto r = random();
				if (r % 9876 == 0) printf("%u\n", r);
				return true;
			},
			50, 0));
	}

	std::this_thread::sleep_for(std::chrono::seconds(10));
	return 0;
}