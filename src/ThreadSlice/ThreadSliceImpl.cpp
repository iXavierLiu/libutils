#include "ThreadSliceImpl.h"

#include <algorithm>
using namespace libutils;

void ThreadSliceImpl::Job()
{
	while (Daemon::IsRunnable()) { func(argsPack); }
}

ThreadSliceImpl::ThreadSliceImpl(FuncType func, ArgsPackType argsPack) : Daemon(this), func(func), argsPack(argsPack)
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
