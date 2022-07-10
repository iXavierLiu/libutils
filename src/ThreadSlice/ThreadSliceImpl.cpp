#include "ThreadSliceImpl.h"

#include <algorithm>
using namespace libutils;

void ThreadSliceImpl::Job()
{
	while (Daemon::IsRunnable())
	{
		std::unique_lock<std::mutex> lck(cvMtx);
		if (!cv.wait_for(lck, std::chrono::milliseconds(100), [this]() -> bool { return cvThreadId; })) continue;

		Delete(cvThreadId);
		cvThreadId = 0;
		cv.notify_all();
	}

	std::lock_guard<std::mutex> threadLck(threadsMtx);
	while (!threads.empty())
	{
		std::unique_lock<std::mutex> lck(cvMtx);
		cv.wait(lck, [this]() -> bool { return cvThreadId; });
		DeleteUnsafe(cvThreadId);
		cvThreadId = 0;
		cv.notify_all();
	}
}

ThreadSliceImpl::ThreadSliceImpl(FuncType func, ArgsPackType argsPack) : Daemon(this), func(func), argsPack(argsPack)
{
	Daemon::Start();
}

ThreadSliceImpl::~ThreadSliceImpl()
{
	Daemon::Stop();
}

bool ThreadSliceImpl::Add(uint8_t count)
{
	std::lock_guard<std::mutex> lck(threadsMtx);
	for (int i = 0; i < count; ++i)
	{
		std::thread th = std::thread(&ThreadSliceImpl::ThreadJob, this);
		threads.insert({ hash(th.get_id()), std::move(th) });
	};
	return true;
}

uint32_t ThreadSliceImpl::GetCount()
{
	std::lock_guard<std::mutex> lck(threadsMtx);
	return threads.size();
}

void ThreadSliceImpl::ThreadJob()
{
	while (Daemon::IsRunnable())
	{
		if (!func(argsPack)) break;
	}

	std::unique_lock<std::mutex> lck(cvMtx);
	cv.wait(lck, [this]() -> bool { return !cvThreadId; });

	cvThreadId = hash(std::this_thread::get_id());
	cv.notify_one();
}

void ThreadSliceImpl::Delete(uint32_t threadId)
{
	std::lock_guard<std::mutex> lck(threadsMtx);
	return DeleteUnsafe(threadId);
}

void ThreadSliceImpl::DeleteUnsafe(uint32_t threadId)
{
	auto it = threads.find(threadId);

	if (threads.end() == it) return;
	if (it->second.joinable()) it->second.join();
	threads.erase(it);
}
