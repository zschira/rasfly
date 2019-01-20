#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <functional>
#include <array>
#include <vector>
#include "hash.h"

const int NUM_SETTINGS = 10;

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

	class config {
	public:
		config(); 
		config(std::string fname);
		~config();
		config_struct readConfig(); 

	private:
		std::array<std::string, NUM_SETTINGS> settingNames = {"pins", "esc_protocol", "imu_driver", "mass", "motor_radius", "mass_moment_inertia", "kp", "kd", "max_thrust", "frame_type"};
		std::array<settings, NUM_SETTINGS> settingVals = {PINS, ESC_PROTOCOL, IMU_DRIVER, MASS, MOTOR_RADIUS, MASS_MOMENT_INERTIA, PROP_GAIN, DERIV_GAIN, MAX_THRUST, FRAME_TYPE};
		std::string config_name;
		hash<std::string, settings, NUM_SETTINGS> table;
		void processSetting(settings setting, std::string value, config_struct &gpio_pins);
		void createHash();
		std::vector<std::string> processList(std::string tuple);
	};
}

#endif