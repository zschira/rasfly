#ifndef _IMU_H_
#define _IMU_H_

#include "rasfly_types.h"

namespace rasfly {
	enum driver_types {
		PYTHON,
		SHARED_OBJECT	
	};
	int loadIMU(hardware &rasfly, const char *driver_name, driver_types driver);
}


#endif