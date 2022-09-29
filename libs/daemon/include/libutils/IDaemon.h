#pragma once

namespace libutils {

class IDaemon
{
	friend class DaemonImpl;

private:
	virtual void Job() = 0;
};

}  // namespace libutils