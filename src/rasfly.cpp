#include "rasfly.h"
#include <fstream>
#include <string>

template <typename IMU>
int rasfly<IMU>::initESC(int pin1, int pin2, int pin3, int pin4) {
	int rate, range;
	//Initialize gpio library 
	if (gpioInitialise() < 0) {
		std::cout << "GPIO INITIALIZE FAILURE\n" ;
		return 1;
	}
	// Setup out put rate 
	switch(gpio_pins.esc_protocol) {
		case ESC_PWM:
			rate = 50; //Hz 	
			range = 4000;
			break;
	}
	for(int pin : gpio_pins.pins) {
		if(rate != gpioSetPWMfrequency(pin, rate)) {
			std::cout << "GPIO INITIALIZE FAILURE\n";
			return 1;
		}
		// Set range
		if(range != gpioSetPWMrange(pin, range)) {
			std::cout << "GPIO INITIALIZE FAILURE\n";
			return 1;
		}
	}
}

template <typename IMU>
int rasfly<IMU>::readConfig() {
	std::ifstream config("rasfly.config");
	std::string line, word; 
	while(std::getline(config, line)) {
		std::getline(line, word);	
		if(word == "PINS") {

		} else if(word == "ESC_PROTOCOL") {
				
		}
	}
}