#pragma once

namespace libutils {

class IDaemon
{
public:
	virtual void Job() = 0;
};

}  // namespace libutils