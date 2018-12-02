#ifndef _IMU_H_
#define _IMU_H_

#include "rasfly_types.h"
#include <Python.h>

namespace rasfly {
	enum driver_types {
		PYTHON,
		SHARED_OBJECT	
	};
	class imu {
	public:
		~imu();
		int loadIMU(hardware &raspi, const char *driver_name, driver_types driver);
		int getState(state &rasfly_state);
	private:	
		int loadPython(hardware &raspi, const char *driver_name);
		PyObject *pFunc, *pModule;
		driver_types imu_driver;
	};

}


#endif