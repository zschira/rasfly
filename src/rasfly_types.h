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

	struct imu_struct {
		vec3 orientation;
		vec3 accel;
		vec3 gyro;
	};

	struct state {
		state() {
			euler = Eigen::Vector3f::Zero();
			accel = Eigen::Vector3f::Zero();
			gyro = Eigen::Vector3f::Zero();
			thrust = 0;
		}
		Eigen::Vector3f euler;
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

	struct gains {
		float roll;
		float pitch;
		float yaw;
	};

	struct config_struct {
		config_struct() {moments = Eigen::Matrix3f::Zero();}
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
		gains proportional;
		gains derivative;
		gains integral;
		float max_thrust;			// N
		Eigen::Matrix3f moments;	// [kg*m^2]
		// Motor torque contributions
		torques tau_x;
		torques tau_y;
		torques tau_z;
	};
}
#endif