#include "rasfly_config.h"

static inline void char_trim(std::string &s, const char delim) {
    	s.erase(std::find_if(s.begin(), s.end(), [&](int ch) {
        	return (ch == delim);
    	}), s.end());
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

rasfly::config::config() {
	std::string home = getenv("HOME");
	config_name = home + "/rasfly.config";
}

rasfly::config::config(std::string fname) {
	config_name = fname;
}

rasfly::config::~config() {}

void rasfly::config::readConfig(hardware &gpio_pins) {
	std::ifstream config_file(config_name);
	std::string line; 
	std::smatch m;
	auto r = std::regex ("a");
	while(std::getline(config_file, line)) {
		char_trim(line, '#');
		std::transform(line.begin(), line.end(), line.begin(), ::tolower);
		for(auto const &setting : setting_map) {
			std::regex_search(line, m, setting.second);
			if( !m.empty()) {
				processSetting(setting.first, m.suffix().str(), gpio_pins);
			}
		}
	}	
}


void rasfly::config::processSetting(settings setting, std::string value, hardware &gpio_pins) {
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
			rtrim(value);
			if(value == "pwm") {
				gpio_pins.protocol = ESC_PWM;
				gpio_pins.esc_range = 4000;
				gpio_pins.esc_rate = 50;
			} else if(value == "oneshot_125") {
				gpio_pins.protocol = ONESHOT_125;	
				gpio_pins.esc_rate = 50 * 8;
			} else if(value == "oneshot_42") {
				gpio_pins.protocol = ONESHOT_42;
			} else {
				std::cout << "Invalid ESC protocol: Using standard PWM\n";
				gpio_pins.protocol = ESC_PWM;
			}
			break;
		}
		case IMU_DRIVER: {
			rtrim(value);
			std::string path, driver_type;
			std::istringstream ss(value);
			std::getline(ss, path, ',');
			std::getline(ss, driver_type);
			char_trim(driver_type, '}');
			std::cout << driver_type;
		}
	}
}