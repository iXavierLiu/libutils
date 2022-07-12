#pragma once

#include "libutils/IDaemon.h"

namespace libutils {

class DaemonImpl;

class Daemon
{
public:
	Daemon(IDaemon* ptr);
	virtual ~Daemon();

protected:
	virtual bool Start();
	virtual bool Stop(bool sync = true);
	bool IsRunnable();

private:
	DaemonImpl* impl = nullptr;
};

}  // namespace libutils