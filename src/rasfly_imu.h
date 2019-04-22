#ifndef _IMU_H_
#define _IMU_H_

#include "rasfly_types.h"
#include <Python.h>
#include <memory>

namespace rasfly {
	class imu {
	public:
		virtual ~imu() {};
		imu(const char *driver_path) {};
		imu() {};
		virtual int getState(state &rasfly_state);
		static std::unique_ptr<imu> imu_factory(const driver_types imu_type, const char *driver_path);
	};

	class PythonDriver : public imu {
	public:
		~PythonDriver();
		PythonDriver(const char *driver_path);
		int getState(state &rasfly_state) override;
	private:
		PyObject *driver_obj, *pFunc, *pModule;
	};

}


#endif