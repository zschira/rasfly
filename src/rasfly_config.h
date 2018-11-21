#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <regex>
#include <stdlib.h>

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
		std::map<settings, std::regex> setting_map {
			{PINS, std::regex ("pins[ \t]*=[ \t]*")},
			{ESC_PROTOCOL, std::regex ("esc_protocol[ \t]*=[ \t]*")},
			{IMU_DRIVER, std::regex ("imu_driver[ \t]*=[ \t]*[{]")}
		};
		std::string config_name;
		void processSetting(settings setting, std::string value, hardware &gpio_pins);
	};
}

#endif