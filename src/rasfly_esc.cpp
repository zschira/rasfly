#include "rasfly_esc.h"

#include <iostream>
#include <pigpio.h>

rasfly::esc::esc(int pin, int rate, int range) {
	if (gpioInitialise() < 0) {
		std::cout << "GPIO INITIALIZE FAILURE\n";
		return;
	}
	if(rate != gpioSetPWMfrequency(pin, rate)) {
		std::cout << "GPIO INITIALIZE FAILURE\n";
		// TODO: failure handle
	}
	// Set range
	if(range != gpioSetPWMrange(pin, range)) {
		std::cout << "GPIO INITIALIZE FAILURE\n";
		// TODO: failure handle
	}
	gpio_pin = pin;
	// TODO: success handle
}

rasfly::esc::~esc() {}

int rasfly::esc::setThrottle(unsigned throttle) {
	unsigned duty, zero_throttle = range / 20;
	duty = zero_throttle + (zero_throttle * throttle) / 2048;
	unsigned err = gpioPWM(gpio_pin, duty);	
	if(!err) {
		return 0;
	} else if(err == PI_BAD_USER_GPIO) {
		std::cout << "BAD GPIO IN SET THROTTLE\n" ;
	} else if(err == PI_BAD_DUTYCYCLE) {
		std::cout << "BAD DUTYCYCLE IN SET THROTTLE\n" ;
	}
	return 1;
}