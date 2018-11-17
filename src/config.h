#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>

class config {
public:
	config() { }

	enum settings {
		PINS,
		ESC_PROTOCOL
	};

	void readConfig() {
		std::ifstream config_file("rasfly.config");
		std::string line; 
		std::smatch m;
		auto r = std::regex ("a");
		while(std::getline(config_file, line)) {
			comment_trim(line);
			std::transform(line.begin(), line.end(), line.begin(), ::tolower);
			for(auto const &setting : setting_map) {
				std::regex_search(line, m, setting.second);
				if( !m.empty()) {
					processSetting(setting.first, m.suffix().str());
				}
			}
		}	
	}

private:
	std::map<settings, std::regex> setting_map {
		{PINS, std::regex ("pins[ \t]*=[ \t]*")},
		{ESC_PROTOCOL, std::regex ("esc_protocol[ \t]*=[ \t]*")}
	};

	void processSetting(settings setting, std::string value) {
		switch(setting) {
			case PINS:
					std::cout<<value << "\n";	
				break;
			case ESC_PROTOCOL:
				std::cout << value << "\n";
				break;
		}
	}

	static inline void comment_trim(std::string &s) {
    	s.erase(std::find_if(s.begin(), s.end(), [](int ch) {
        	return (ch == '#');
    	}), s.end());
	}
};

#endif