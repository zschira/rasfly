#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <functional>

#define NUM_SETTINGS 3

#include "rasfly_types.h"
namespace rasfly { 
	class config {
	public:
		config(); 
		config(std::string fname);
		~config();
		enum settings {
			PINS,
			ESC_PROTOCOL,
			IMU_DRIVER
		};

		void readConfig(hardware &gpio_pins); 
		void readConfig(hardware &gpio_pins, std::string fname);

	private:
		settings settings_arr[NUM_MOTORS];
		std::string config_name;
		std::hash<std::string> setting_hash;
		void processSetting(settings setting, std::string value, hardware &gpio_pins);
		void createHash();
	};
}

#endif