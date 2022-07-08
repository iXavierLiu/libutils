#pragma once

#include <chrono>
#include <type_traits>

namespace std {
template <class T>
class function;

template <class _Ty>
class shared_ptr;
}  // namespace std

namespace libutils {

class TimerTaskImpl;
class TimerTask
{
public:
	using Ptr = std::shared_ptr<TimerTask>;

	friend class TimerTaskPoolImpl;

public:
	virtual ~TimerTask();

	/// <summary>
	///
	/// </summary>
	/// <param name="func">回调返回值暂时无用</param>
	/// <param name="intervalMilSec"></param>
	/// <param name="times"></param>
	/// <param name="immediately"></param>
	/// <returns></returns>
	static Ptr create(std::function<bool()> func, uint64_t intervalMilSec, uint64_t times = 1, bool immediately = false);

	/// <summary>
	///
	/// </summary>
	/// <param name="time">当前时间</param>
	/// <returns>是否执行完毕，以后不再执行</returns>
	bool TimeSlice(const std::chrono::steady_clock::time_point& time);

private:
	TimerTask(std::function<bool()> func, uint64_t intervalMilSec, uint64_t times, bool immediately);

private:
	TimerTaskImpl* impl;
};

}  // namespace libutils