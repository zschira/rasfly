#include "rasfly_esc.h"
#include "rasfly_logs.h"

#include <pigpio.h>

rasfly::esc::esc(int pin, int rate, int range) {
	if (gpioInitialise() < 0) {
		return;
	}
	if(rate != gpioSetPWMfrequency(pin, rate)) {
		rasfly::logger.write("GPIO FREQUENCY FAILURE");
		// TODO: failure handle
	}
	// Set range
	if(range != gpioSetPWMrange(pin, range)) {
		rasfly::logger.write("GPIO RANGE FAILURE");
		// TODO: failure handle
	}
	gpio_pin = pin;
	// TODO: success handle
	steps = 2048;
}


int rasfly::esc::setThrottle(float throttle) {
	unsigned throttle_converted = (unsigned) (throttle * steps);
	unsigned duty, zero_throttle = range / 20;
	duty = zero_throttle + (zero_throttle * throttle_converted) / steps;
	unsigned err = gpioPWM(gpio_pin, duty);	
	if(!err) {
		return 0;
	} else if(err == PI_BAD_USER_GPIO) {
		rasfly::logger.write("CAN'T SET THROTTLE: BAD GPIO");
		// TODO: failure handle
	} else if(err == PI_BAD_DUTYCYCLE) {
		rasfly::logger.write("CAN'T SET THROTTLE: BAD DUTYCYCLE");
		// TODO: failure handle
	}
	return 1;
}