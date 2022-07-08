#include "libutils/TimerTask.h"
#include "libutils/TimerTaskPool.h"

#include <thread>

int main(int argc, char* argv[])
{
	using namespace libutils;

	auto pool = TimerTaskPool::create();

	double times = 0;
	pool->Add(TimerTask::create(
		[&times]() -> bool {
			printf("%f, 1000ms\n", ++times);
			return true;
		},
		1000, 10));

	pool->Add(TimerTask::create(
		[&times]() -> bool {
			printf("%f, 500ms\n", times += 0.1);
			return true;
		},
		500, 0));

	TimerTask::Ptr p;
	pool->Add(p = TimerTask::create(
				  [&times]() -> bool {
					  printf("%f, 50ms\n", times += 0.000001);
					  return true;
				  },
				  50, 0));

	std::this_thread::sleep_for(std::chrono::seconds(5));
	pool->Delete(p);
	std::this_thread::sleep_for(std::chrono::seconds(3));
	return 0;
}