#ifndef _RASFLY_FILTER_H_
#define _RASFLY_FILTER_H_

#include <functional>
#include "rasfly_types.hpp"

namespace rasfly {

class Filter {
public:
	std::function<State(State)> filterState;
};

}

#endif