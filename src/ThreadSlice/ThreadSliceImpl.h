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
	using ArgsPackType = std::shared_ptr<void>;
	using FuncType = std::function<bool(ArgsPackType)>;

protected:
	// 通过 IDaemon 继承
	virtual void Job() override;

public:
	ThreadSliceImpl(FuncType func, ArgsPackType ctx);
	virtual ~ThreadSliceImpl();
	bool Add(uint8_t count = 1);

private:
	FuncType func;
	std::list<std::thread> threads;
	ArgsPackType argsPack = nullptr;
};

};	// namespace libutils