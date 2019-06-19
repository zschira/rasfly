#ifndef _RASFLY_INPUTS_H_
#define _RASFLY_INPUTS_H_

#include <functional>
#include "rasfly_types.hpp"

namespace rasfly {

class Inputs {
public:
	std::function<State()> getPilotInput;
};

}

#endif