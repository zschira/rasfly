#ifndef _IMU_H_
#define _IMU_H_

#include "rasfly_types.h"
#include <Python.h>

namespace rasfly {
	class imu {
	public:
		~imu();
		int loadIMU(hardware &raspi);
		int getState(state &rasfly_state);
	private:	
		int loadPython(hardware &raspi, const char *driver_name);
		PyObject *driver_obj, *pFunc, *pModule;
		driver_types imu_driver;
	};

}


#endif