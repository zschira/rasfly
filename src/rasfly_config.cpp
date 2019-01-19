#include "rasfly_config.h"
#include "rasfly_imu.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include "string_utils.h"


rasfly::config::config() {
	std::string home = getenv("HOME");
	config_name = home + "/rasfly.config";
}

rasfly::config::config(std::string fname) {
	config_name = fname;
}

rasfly::config::~config() {}

void rasfly::config::createHash() {
	table.create(settingNames.begin(), settingNames.end(), settingVals.begin(), settingVals.end());
}

rasfly::config_struct rasfly::config::readConfig() {
	config_struct configuration;
	createHash();
	std::ifstream config_file(config_name);
	std::string line, setting_str, setting_val; 
	while(std::getline(config_file, line)) {
		char_trim(line, '#');
		std::transform(line.begin(), line.end(), line.begin(), ::tolower);
		erase_space(line);
		std::istringstream ss(line);
		std::getline(ss, setting_str, '=');
		settings val;
		int success = table.access(setting_str, val);
		if(success < 0 || setting_str.empty()) {
			continue;
		}
		// Get value
		std::getline(ss, setting_val);
		processSetting(val, setting_val, configuration);
	}	
	return configuration;
}


void rasfly::config::processSetting(settings setting, std::string value, config_struct &configuration) {
	switch(setting) {
		case PINS: {
			std::vector<std::string> pins = processList(value);
			for(int i=0; i<4; ++i) {
				configuration.esc_pins[i] = std::stoi(pins[i]);
			}
			break;
		}
		case ESC_PROTOCOL: {
			if(value == "pwm") {
				configuration.protocol = ESC_PWM;
				configuration.esc_range = 4000;
				configuration.esc_rate = 50;
			} else if(value == "oneshot_125") {
				configuration.protocol = ONESHOT_125;	
				configuration.esc_rate = 50 * 8;
			} else if(value == "oneshot_42") {
				configuration.protocol = ONESHOT_42;
			} else {
				std::cout << "Invalid ESC protocol: Using standard PWM\n";
				configuration.protocol = ESC_PWM;
				configuration.esc_range = 4000;
				configuration.esc_rate = 50;
			}
			break;
		}
		case IMU_DRIVER: {
			std::string path, driver_type;
			std::istringstream ss(value);
			std::getline(ss, path, ',');
			erase_char(path, '{');
			configuration.imu_path = new char[strlen(path.c_str())];
			strcpy(configuration.imu_path, path.c_str());
			std::getline(ss, driver_type);
			char_trim(driver_type, '}');
			if(driver_type == "python") {
				configuration.imu_driver = PYTHON;
			} else if(driver_type == "shared_object") {
				configuration.imu_driver = SHARED_OBJECT;
			}
			break;
		}
		case MASS: {
			configuration.mass = std::stof(value);
			break;
		}
		case MOTOR_RADIUS: {
			configuration.motor_radius = std::stof(value);
			break;
		}
		case MASS_MOMENT_INERTIA: {
			std::vector<std::string> moments = processList(value);
			if(value.length() == 3) {
				for(int i=0; i<3; ++i) {
					configuration.moments(i, i) = std::stof(moments[i]);
				}
			}
			break;
		}
		case PROP_GAIN: {
			configuration.prop_gain = std::stof(value);
			break;
		}
		case DERIV_GAIN: {
			configuration.deriv_gain = std::stof(value);
			break;
		}
		case MAX_THRUST: {
			configuration.max_thrust = std::stof(value);
			break;
		}
		case FRAME_TYPE: {
			if(value == "x") {
				configuration.roll.M1 = 1; configuration.roll.M2 = 1; configuration.roll.M3 = -1; configuration.roll.M4 = -1;
				configuration.pitch.M1 = 1; configuration.pitch.M2 = -1; configuration.pitch.M3 = -1; configuration.pitch.M4 = 1;
				configuration.yaw.M1 = 1; configuration.yaw.M2 = -1; configuration.yaw.M3 = 1; configuration.yaw.M4 = -1;
			} else if(value == "plus") {
				configuration.roll.M1 = 0; configuration.roll.M2 = 1; configuration.roll.M3 = 0; configuration.roll.M4 = -1;
				configuration.pitch.M1 = 1; configuration.pitch.M2 = 0; configuration.pitch.M3 = -1; configuration.pitch.M4 = 0;
				configuration.yaw.M1 = 1; configuration.yaw.M2 = -1; configuration.yaw.M3 = 1; configuration.yaw.M4 = -1;
			}
		}
	}
}

std::vector<std::string> rasfly::config::processList(std::string tuple) {
	std::vector<std::string> values;
	std::string val;
	std::istringstream ss(tuple);
	while(std::getline(ss, val, ',')) {
		values.push_back(val);
	}
	return values;
}