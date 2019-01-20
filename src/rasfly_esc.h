#ifndef _ESC_H_
#define _ESC_H_

#include "rasfly_types.h"
namespace rasfly {
	class esc {
	public:
		esc(int pin, int rate, int range);
		esc(){}
		~esc() {}
		int setThrottle(float throttle);
	private:
		int gpio_pin;
		int rate;
		int range;
		unsigned steps;
	};
}

#endif