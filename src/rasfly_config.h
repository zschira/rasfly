#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <functional>
#include <array>
#include <vector>
#include "hash.h"

constexpr int NUM_SETTINGS = 10;

#include "rasfly_types.h"
namespace rasfly { 
	enum settings {
		PINS,
		ESC_PROTOCOL,
		IMU_DRIVER,
		MASS,
		MOTOR_RADIUS,
		MASS_MOMENT_INERTIA,
		PROP_GAIN,
		DERIV_GAIN,
		MAX_THRUST,
		FRAME_TYPE
	};

	config_struct readConfig(std::string config_name); 

		

}

#endif