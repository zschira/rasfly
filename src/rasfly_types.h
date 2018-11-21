#ifndef _TYPES_H_
#define _TYPES_H_

#include <vector>
namespace rasfly {
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
		int esc_rate, esc_range;
		int (*imu)(state);	
	};
}
#endif