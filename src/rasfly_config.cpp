#include "rasfly_config.h"
#include "rasfly_imu.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include "utils.h"


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

void rasfly::config::readConfig(hardware &raspi) {
	createHash();
	std::ifstream config_file(config_name);
	std::string line, setting_str, setting_val; 
	while(std::getline(config_file, line)) {
		char_trim(line, '#');
		std::transform(line.begin(), line.end(), line.begin(), ::tolower);
		erase_space(line);
		std::istringstream ss(line);
		std::getline(ss, setting_str, '=');
		settings val = table.access(setting_str);
		if(val < 0 || val >= NUM_SETTINGS) {
			continue;
		}
		// Get value
		std::getline(ss, setting_val);
		processSetting(val, setting_val, raspi);
	}	
}


void rasfly::config::processSetting(settings setting, std::string value, hardware &raspi) {
	switch(setting) {
		case PINS: {
			std::string pin_str;
			std::istringstream ss(value);
			int counter = 0;
			while(std::getline(ss, pin_str, ',')) {
				if(counter == NUM_MOTORS) {
					std::cout << "RASFLY only supports 4 motors\n";
					break;
				}
				raspi.esc_pins[counter] = std::stoi(pin_str);
				++counter;
			}
			break;
		}
		case ESC_PROTOCOL: {
			if(value == "pwm") {
				raspi.protocol = ESC_PWM;
				raspi.esc_range = 4000;
				raspi.esc_rate = 50;
			} else if(value == "oneshot_125") {
				raspi.protocol = ONESHOT_125;	
				raspi.esc_rate = 50 * 8;
			} else if(value == "oneshot_42") {
				raspi.protocol = ONESHOT_42;
			} else {
				std::cout << "Invalid ESC protocol: Using standard PWM\n";
				raspi.protocol = ESC_PWM;
				raspi.esc_range = 4000;
				raspi.esc_rate = 50;
			}
			break;
		}
		case IMU_DRIVER: {
			std::string path, driver_type;
			std::istringstream ss(value);
			std::getline(ss, path, ',');
			erase_char(path, '{');
			raspi.imu_path = new char[strlen(path.c_str())];
			strcpy(raspi.imu_path, path.c_str());
			std::getline(ss, driver_type);
			char_trim(driver_type, '}');
			if(driver_type == "python") {
				raspi.imu_driver = PYTHON;
			} else if(driver_type == "shared_object") {
				raspi.imu_driver = SHARED_OBJECT;
			}
		}
	}
}