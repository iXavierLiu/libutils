#include "ThreadSliceImpl.h"

#include <algorithm>
using namespace libutils;

void ThreadSliceImpl::Job()
{
	while (Daemon::IsRunnable()) { func(sharedContex); }
}

ThreadSliceImpl::ThreadSliceImpl(std::function<bool(SharedContexPtr ctx)> func, SharedContexPtr ctx) : Daemon(this), func(func), sharedContex(ctx)
{
	Daemon::Start();
}

ThreadSliceImpl::~ThreadSliceImpl()
{
	Daemon::Stop();
	std::for_each(threads.begin(), threads.end(), [](std::thread& t) {
		if (t.joinable()) t.join();
	});
}

bool ThreadSliceImpl::Add(uint8_t count)
{
	for (int i = 0; i < count; ++i) threads.emplace_back(std::thread(&ThreadSliceImpl::Job, this));
	return true;
}
