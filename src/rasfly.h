#ifndef RASFLY_H
#define RASFLY_H

#include <iostream>
#include <memory>
#include <vector>
#include <thread>

#include "rasfly_config.h"
#include "rasfly_types.h"

class rasfly {
public:
	rasfly(); 
	~rasfly();
	int initESC(int pin1, int pin2, int pin3, int pin4);
private:
	int setThrottle();
	hardware gpio_pins;
	std::shared_ptr<state> imu_state;
};

#endif