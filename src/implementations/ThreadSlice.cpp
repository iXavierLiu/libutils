#include "libutils/ThreadSlice.h"

#include "ThreadSlice/ThreadSliceImpl.h"

using namespace libutils;

libutils::ThreadSlice::~ThreadSlice()
{
	delete impl;
}

ThreadSlice::Ptr ThreadSlice::create(std::function<bool(SharedContex)> func, SharedContex ctx)
{
	return Ptr(new ThreadSlice(func, ctx));
}
bool libutils::ThreadSlice::Add(uint8_t count)
{
	return impl->Add(count);
};

libutils::ThreadSlice::ThreadSlice(std::function<bool(SharedContex)> func, SharedContex ctx) : impl(new ThreadSliceImpl(func, ctx)) {}
