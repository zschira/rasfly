#ifndef _RASFLY_MOTORS_H_
#define _RASFLY_MOTORS_H_

#include <functional>
#include "rasfly_types.hpp"

namespace rasfly {

//////////////////////////////////////////////////////////////////////////////////
/// @class Motors
//////////////////////////////////////////////////////////////////////////////////
class Motors {
public:
	std::function<void(Thrust_4M)> setThrust;
};

}

#endif