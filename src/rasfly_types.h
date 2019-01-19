#ifndef _TYPES_H_
#define _TYPES_H_

#include <array>
#include <string>
#include <Eigen/Dense>

#define NUM_MOTORS 4

namespace rasfly {
	enum esc_protocol{ESC_PWM, ONESHOT_125, ONESHOT_42};

	enum driver_types { PYTHON, SHARED_OBJECT	};

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
		Eigen::Vector3f accel;
		Eigen::Vector3f gyro;
		float thrust;
	};

	struct thrusts {
		// Desired thrust for each motor in N
		float T1;
		float T2;
		float T3;
		float T4;
	};

	struct torques {
		// Denotes whether each motor contributes to a torque
		float M1;
		float M2;
		float M3;
		float M4;
	};

	struct config_struct {
		~config_struct() { delete[] imu_path;}
		int esc_pins[NUM_MOTORS];
		int num_esc;
		esc_protocol protocol;
		int esc_rate, esc_range;
		driver_types imu_driver;
		char *imu_path;
		// Physical characteristics
		float mass;					// [kg]
		float motor_radius;			// [m]
		float prop_gain;			// dimensionless
		float deriv_gain;			// dimensionless
		float max_thrust;			// N
		Eigen::Matrix3f moments;	// [kg*m^2]
		// Motor torque contributions
		torques roll;
		torques pitch;
		torques yaw;
	};
}
#endif