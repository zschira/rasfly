#ifndef _RASFLY_H_
#define _RASFLY_H_

#include <memory>
#include "rasfly_plugins.hpp"
#include "rasfly_types.hpp"
#include "rasfly_imu.hpp"
#include "rasfly_controller.hpp"
#include "rasfly_filter.hpp"
#include "rasfly_motors.hpp"
#include "rasfly_inputs.hpp"

namespace rasfly {
//////////////////////////////////////////////////////////////////////////////////
/// @class rasfly_app
//////////////////////////////////////////////////////////////////////////////////
class rasfly_app {
public:
    rasfly_app();
    void BindCallbacks();
    void run();
private:
    Plugins plugins;                                        // Plugins class
    // Each class contains a callback function
    std::unique_ptr<IMU> _imu;                                // Pointer to imu class (gets state of copter)
    std::unique_ptr<Filter> _filter;                        // Pointer to filter class (apply kalman filter)
    std::unique_ptr<Controller> _controller;                // Pointer to controller class (Calculate thrust based on controls)
    std::unique_ptr<Motors> _motors;                        // Pointer to motors class (send thrust controls to motors)
    std::unique_ptr<Inputs> _inputs;                        // Pointer to inputs class (Get pilot inputs)
};

}

#endif