#include "rasfly_control.h"

rasfly::controller::controller(config_struct configuration) {
	// Set physical parameters used by PID controller
	mass = configuration.mass;
	motor_radius = configuration.motor_radius;
	moments = configuration.moments;
	max_thrust = configuration.max_thrust;
	// Set gain vectors
	Kp(0) = configuration.proportional.roll;
	Kp(1) = configuration.proportional.pitch;
	Kp(2) = configuration.proportional.yaw;
	Kd(0) = configuration.derivative.roll;
	Kd(1) = configuration.derivative.pitch;
	Kd(2) = configuration.derivative.yaw;
	// Set torque contribution matrix
	torque_state(0, 0) = configuration.tau_x.M1; torque_state(0, 1) = configuration.tau_x.M2; torque_state(0, 2) = configuration.tau_x.M3; torque_state(0, 3) = configuration.tau_x.M4;
	torque_state(1, 0) = configuration.tau_y.M1; torque_state(1, 1) = configuration.tau_y.M2; torque_state(1, 2) = configuration.tau_y.M3; torque_state(1, 3) = configuration.tau_y.M4;
	torque_state(2, 0) = configuration.tau_z.M1; torque_state(2, 1) = configuration.tau_z.M2; torque_state(2, 2) = configuration.tau_z.M3; torque_state(2, 3) = configuration.tau_z.M4;
	torque_state(3, 0) = 1; 					 torque_state(3, 1) = 1; 					  torque_state(3, 2) = 1; 					   torque_state(3, 3) = 1;
	// Construct linear solver using LDLT decomposition
	ldlt.compute(torque_state);
}

rasfly::thrusts rasfly::controller::calculateThrust(state measured, state trim) {
	thrusts thrust_struct;
	// Compute state errors
	Eigen::Vector3f delta_angle = measured.euler - trim.euler;
	Eigen::Vector3f delta_angle_rate = measured.gyro - trim.gyro;
	Eigen::Vector3f delta_accel = measured.accel - trim.accel;
	// Compute control terms
	Eigen::Vector3f control_func = Kd.array() * delta_angle_rate.array() + Kp.array() + delta_angle.array();
	control_func = moments * control_func;
	control_func(0) /= motor_radius; control_func(1) /= motor_radius;
	Eigen::Vector4f b(control_func(0), control_func(1), control_func(2), trim.thrust);
	// Solve for current thrusts as a percentage of max thrust
	Eigen::Vector4f thrusts = ldlt.solve(b);
	thrust_struct.T1 = thrusts(0) / max_thrust;
	thrust_struct.T2 = thrusts(1) / max_thrust;
	thrust_struct.T3 = thrusts(2) / max_thrust;
	thrust_struct.T4 = thrusts(3) / max_thrust;
	return thrust_struct;
}