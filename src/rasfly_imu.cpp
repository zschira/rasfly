#include "rasfly_imu.h"
#include <iostream>


rasfly::imu::~imu() {
	if(imu_driver == PYTHON) {
		Py_DECREF(pFunc);
		Py_DECREF(pModule);
	}
}

int rasfly::imu::loadIMU(hardware &raspi){
	int success = 0;
	imu_driver = raspi.imu_driver;
	if(imu_driver == PYTHON) {
		success = loadPython(raspi, raspi.imu_path); 	
	}
	return 0;
}

int rasfly::imu::loadPython(hardware &raspi, const char *driver_name) {
	PyObject *pName;
	const char *func_name = "getState";

	// Initialize python interpreter
	Py_Initialize();
	pName = PyUnicode_DecodeFSDefault(driver_name);

	// Load diver
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\"/home/alarm/rasfly\")"); 
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);	
	if(pModule == NULL) {
		PyErr_Print();
		return 1;
	}

	// Load function
	pFunc = PyObject_GetAttrString(pModule, func_name);	
	if(!pFunc || !PyCallable_Check(pFunc)) {
		PyErr_Print();
		return 1;
	}

	return 0;
}

int rasfly::imu::getState(state &rasfly_state) {
	PyObject *pArgs, *pValue;
	if(imu_driver == PYTHON) {
		PyObject_CallObject(pFunc, pArgs);	
	}
	return 0;
}