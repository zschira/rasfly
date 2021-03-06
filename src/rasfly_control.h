#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "rasfly_types.h"
#include <Eigen/QR>

namespace rasfly {
	class controller {
	public:
		controller(config_struct configuration);
		thrusts calculateThrust(state measured, state trim);
	private:
		// Gains
		Eigen::Vector3f Kp;
		Eigen::Vector3f Kd;
		// Physical parameters
		float motor_radius;
		float mass;
		float max_thrust;
		Eigen::Matrix3f moments;
		// Motor torque impacts
		Eigen::Matrix4f torque_state;
		Eigen::ColPivHouseholderQR<Eigen::Matrix4f> ldlt;
	};
}

#endif