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

class config {
public:
	config() { }

	enum settings {
		PINS,
		ESC_PROTOCOL
	};

	void readConfig(hardware &gpio_pins) {
		std::string home = getenv("HOME");
		std::ifstream config_file(home + "/rasfly.config");
		std::string line; 
		std::smatch m;
		auto r = std::regex ("a");
		while(std::getline(config_file, line)) {
			comment_trim(line);
			std::transform(line.begin(), line.end(), line.begin(), ::tolower);
			for(auto const &setting : setting_map) {
				std::regex_search(line, m, setting.second);
				if( !m.empty()) {
					processSetting(setting.first, m.suffix().str(), gpio_pins);
				}
			}
		}	
	}

private:
	std::map<settings, std::regex> setting_map {
		{PINS, std::regex ("pins[ \t]*=[ \t]*")},
		{ESC_PROTOCOL, std::regex ("esc_protocol[ \t]*=[ \t]*")}
	};

	void processSetting(settings setting, std::string value, hardware &gpio_pins) {
		switch(setting) {
			case PINS: {
				std::string pin_str;
				std::istringstream ss(value);
				while(std::getline(ss, pin_str, ',')) {
					gpio_pins.pins.push_back(std::stoi(pin_str));
				}
				break;
			}
			case ESC_PROTOCOL: {
				if(value == "pwm") {
					gpio_pins.protocol = ESC_PWM;
					gpio_pins.esc_range = 4000;
					gpio_pins.esc_rate = 50;
				} else if(value == "oneshot_125") {
					gpio_pins.protocol = ONESHOT_125;	
				} else if(value == "oneshot_42") {
					gpio_pins.protocol = ONESHOT_42;
				} else {
					std::cout << "Invalid ESC protocol: Using standard PWM\n";
					gpio_pins.protocol = ESC_PWM;
				}
				break;
			}
		}
	}

	static inline void comment_trim(std::string &s) {
    	s.erase(std::find_if(s.begin(), s.end(), [](int ch) {
        	return (ch == '#');
    	}), s.end());
	}
};

#endif