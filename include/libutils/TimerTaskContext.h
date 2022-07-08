#pragma once

#include <chrono>
#include <functional>

namespace libutils {

struct TaskContext
{
	std::function<bool()> func = nullptr;
	std::chrono::milliseconds intervalMilSec{ 0 };
	std::chrono::steady_clock::time_point nextTime{};
	uint64_t times = 1;
	bool always = false;
};

}  // namespace libutils