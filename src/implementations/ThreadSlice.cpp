#include "libutils/ThreadSlice.h"

#include "ThreadSlice/ThreadSliceImpl.h"

using namespace libutils;

ThreadSliceProxy::~ThreadSliceProxy()
{
	delete impl;
}

ThreadSliceProxy::Ptr ThreadSliceProxy::create(FuncType func, ArgsPackType argsPack)
{
	return Ptr(new ThreadSliceProxy(func, argsPack));
}
bool ThreadSliceProxy::Add(uint8_t count)
{
	return impl->Add(count);
};

ThreadSliceProxy::ThreadSliceProxy(FuncType func, ArgsPackType argsPack) : impl(new ThreadSliceImpl(func, argsPack)) {}
