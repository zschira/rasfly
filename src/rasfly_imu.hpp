#ifndef _RASFLY_IMU_H_
#define _RASFLY_IMU_H_

#include <functional>
#include "rasfly_types.hpp"

namespace rasfly {

//////////////////////////////////////////////////////////////////////////////////
/// @class IMU
//////////////////////////////////////////////////////////////////////////////////
class IMU {
public:
    IMU() {}
    ~IMU() {}
    std::function<State()> getState;
private:
    State state;
};
}

#endif