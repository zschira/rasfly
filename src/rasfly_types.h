#ifndef _TYPES_H_
#define _TYPES_H_

#include <array>
#include <string>

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
		vec3 accel;
		vec3 gyro;
	};

	typedef struct config_struct {
		~config_struct() { delete[] imu_path;}
		int esc_pins[NUM_MOTORS];
		int num_esc;
		esc_protocol protocol;
		int esc_rate, esc_range;
		driver_types imu_driver;
		char *imu_path;
	}config_struct;
}
#endif