#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <functional>
#include <array>
#include "hash.h"

const int NUM_SETTINGS = 8;

#include "rasfly_types.h"
namespace rasfly { 
	enum settings {
		PINS,
		ESC_PROTOCOL,
		IMU_DRIVER
	};

	class config {
	public:
		config(); 
		config(std::string fname);
		~config();
		config_struct readConfig(); 

	private:
		std::array<std::string, NUM_SETTINGS> settingNames = {"pins", "esc_protocol", "imu_driver", "mass", "motor_radius", "mass_moment_inertia", "prop_gain", "deriv_gain"};
		std::array<settings, NUM_SETTINGS> settingVals = {PINS, ESC_PROTOCOL, IMU_DRIVER};
		std::string config_name;
		hash<std::string, settings, NUM_SETTINGS> table;
		void processSetting(settings setting, std::string value, config_struct &gpio_pins);
		void createHash();
	};
}

#endif