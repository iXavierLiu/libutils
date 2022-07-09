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
	virtual void ThreadJob();
	void Delete(uint32_t threadId);
	void DeleteUnsafe(uint32_t threadId);

private:
	FuncType func;
	ArgsPackType argsPack = nullptr;

	std::hash<std::thread::id> hash;

	std::map<uint32_t, std::thread> threads;
	std::mutex threadsMtx;

	std::condition_variable cv;
	std::mutex cvMtx;
	uint32_t cvThreadId = 0;
};

};	// namespace libutils