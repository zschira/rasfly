#ifndef _RASFLY_CONTROLLER_H_
#define _RASFLY_CONTROLLER_H_

#include <functional>
#include "rasfly_types.hpp"

namespace rasfly {

class Controller {
public:
	Controller() {}
	~Controller() {}
	std::function<Thrust(State)> calcThrust;
private:
};
}

#endif