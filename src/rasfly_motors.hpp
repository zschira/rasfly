#ifndef _RASFLY_MOTORS_H_
#define _RASFLY_MOTORS_H_

#include <functional>
#include <memory>
#include "rasfly_types.hpp"

namespace rasfly {

//////////////////////////////////////////////////////////////////////////////////
/// @class Motors
//////////////////////////////////////////////////////////////////////////////////
class Motors {
public:
    Motors();
    std::function<void(Thrust&)> setThrust;
private:
    struct DefaultMotors;
    std::unique_ptr<DefaultMotors> pImpl;
};

}

#endif