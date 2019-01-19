#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "rasfly_types.h"

namespace rasfly {
	class controller {
	public:
		controller(config_struct configuration);
		thrusts calculateThrust(state measured, state trim);
	private:
		// Gains
		float roll_kd, roll_kp;
		float pitch_kd, pitch_kp;
		float yaw_kd, yaw_kp;
		// Physical parameters
		float motor_radius;
		float mass;
		Eigen::Matrix3f moments;
		// Motor torque impacts
		Eigen::Matrix4f torque_state;
	};
}

#endif