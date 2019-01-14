#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <functional>
#include <array>
#include "hash.h"

const int NUM_SETTINGS = 3;

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
		void readConfig(hardware &gpio_pins); 
		void readConfig(hardware &gpio_pins, std::string fname);

	private:
		std::array<std::string, NUM_SETTINGS> settingNames = {"pins", "esc_protocol", "imu_driver"};
		std::array<settings, NUM_SETTINGS> settingVals = {PINS, ESC_PROTOCOL, IMU_DRIVER};
		std::string config_name;
		hash<std::string, settings, NUM_SETTINGS> table;
		void processSetting(settings setting, std::string value, hardware &gpio_pins);
		void createHash();
	};
}

#endif