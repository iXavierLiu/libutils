#pragma once
#include "libutils/Daemon.h"

#include <functional>
#include <list>
#include <map>
#include <mutex>
#include <thread>

namespace libutils {

class ThreadSliceImpl : protected IDaemon, public Daemon
{
public:
	using SharedContexPtr = void*;

protected:
	// 通过 IDaemon 继承
	virtual void Job() override;

public:
	ThreadSliceImpl(std::function<bool(SharedContexPtr ctx)> func, SharedContexPtr ctx);
	virtual ~ThreadSliceImpl();
	bool Add(uint8_t count = 1);

private:
	std::function<bool(SharedContexPtr ctx)> func;
	std::list<std::thread> threads;
	SharedContexPtr sharedContex = nullptr;
};

};	// namespace libutils