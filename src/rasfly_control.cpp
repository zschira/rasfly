#include "rasfly_control.h"

rasfly::controller::controller(config_struct configuration) {
	// Set physical parameters used by PID controller
	mass = configuration.mass;
	motor_radius = configuration.motor_radius;
	moments = configuration.moments;
	roll_kd = configuration.deriv_gain;
	pitch_kd = configuration.deriv_gain;
	yaw_kd = configuration.deriv_gain;
	roll_kp = configuration.prop_gain;
	pitch_kp = configuration.prop_gain;
	yaw_kp = configuration.prop_gain;
}

rasfly::thrusts rasfly::controller::calculateThrust(state measured, state trim) {
	thrusts thrust_struct;
	return thrust_struct;
}