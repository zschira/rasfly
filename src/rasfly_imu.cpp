#include "rasfly_imu.h"
#include <iostream>
#include <ctime>

int rasfly::imu::getState(state &rasfly_state) {
	return 0;
}

std::unique_ptr<rasfly::imu> rasfly::imu::imu_factory(const driver_types imu_type, const char *driver_path) {
	if(imu_type == PYTHON) {
			return std::make_unique<PythonDriver>(driver_path);
	}
	return std::make_unique<imu>(driver_path);
}