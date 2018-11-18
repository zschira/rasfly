#include "rasfly.h"
#include <fstream>
#include <pigpio.h>
#include <string>

rasfly::rasfly() {
	config conf;
	conf.readConfig(gpio_pins);
}

rasfly::~rasfly() {
}

int rasfly::initESC(int pin1, int pin2, int pin3, int pin4) {
	int rate, range;
	//Initialize gpio library 
	if (gpioInitialise() < 0) {
		std::cout << "GPIO INITIALIZE FAILURE\n" ;
		return 1;
	}

	// Setup out put rate 
	for(int pin : gpio_pins.pins) {
		if(gpio_pins.esc_rate != gpioSetPWMfrequency(pin, gpio_pins.esc_rate)) {
			std::cout << "GPIO INITIALIZE FAILURE\n";
			return 1;
		}
		// Set range
		if(gpio_pins.esc_range != gpioSetPWMrange(pin, gpio_pins.esc_range)) {
			std::cout << "GPIO INITIALIZE FAILURE\n";
			return 1;
		}
	}
	return  0;
}