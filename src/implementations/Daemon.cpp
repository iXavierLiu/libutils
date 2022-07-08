#include "libutils/Daemon.h"

#include "Daemon/DaemonImpl.h"

using namespace libutils;

Daemon::Daemon(IDaemon* ptr) : impl(new DaemonImpl(ptr)) {}

Daemon::~Daemon()
{
	if (impl) delete impl;
}

bool Daemon::Start()
{
	return impl->Start();
}

bool Daemon::Stop(bool sync)
{
	return impl->Stop(sync);
}

bool Daemon::IsRunnable()
{
	return impl->IsRunnable();
}