#include "rasfly_imu.h"
#include <iostream>


rasfly::imu::~imu() {
	if(imu_driver == PYTHON) {
		Py_DECREF(pFunc);
		Py_DECREF(pModule);
	}
}

int rasfly::imu::loadIMU(hardware &raspi, const char *driver_name, driver_types driver){
	int success = 0;
	imu_driver = driver;
	if(driver == PYTHON) {
		success = loadPython(raspi, driver_name); 	
	}
	return 0;
}

int rasfly::imu::loadPython(hardware &raspi, const char *driver_name) {
	PyObject *pName;
	PyObject *pArgs, *pValue;	
	const char *func_name = "getState";

	// Initialize python interpreter
	Py_Initialize();
	pName = PyUnicode_DecodeFSDefault(driver_name);

	// Load diver
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);	
	if(pModule == NULL) {
		std::cout << "Could not load: " << driver_name << "\n";
		return 1;
	}

	// Load function
	pFunc = PyObject_GetAttrString(pModule, func_name);	
	if(!pFunc || !PyCallable_Check(pFunc)) {
		std::cout << "Could not load function: " << func_name << "\n";
		return 1;
	}

	return 0;
}

int rasfly::imu::getState(state &rasfly_state) {
	if(imu_driver == PYTHON) {
		
	}
	return 0;
}