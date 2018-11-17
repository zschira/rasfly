#ifndef RASFLY_H
#define RASFLY_H

#include <iostream>
#include <memory>
#include <vector>
#include <thread>

#include "config.h"

enum esc_protocol{ESC_PWM, ONESHOT_125, ONESHOT_42};

struct quaternion {
	double w;
	double x;
	double y;
	double z;
};

struct vec3 {
	double x;	
	double y;	
	double z;	
};

struct state {
	quaternion orientation;
	vec3 accel;
	vec3 gyro;
};

struct hardware {
	std::vector<int> pins;
	esc_protocol protocol;
	int (*imu)(state);	
};

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