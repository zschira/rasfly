#ifndef _RASFLY_H_
#define _RASFLY_H_

#include <functional>
#include "rasfly_types.hpp"

class IMU
{
public:
	IMU();
	~IMU();
	std::function<State()> getState;
};

#endif