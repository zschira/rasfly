#include "rasfly_imu.h"
#include <iostream>
#include <ctime>


rasfly::imu::~imu() {
	if(imu_driver == PYTHON) {
		Py_DECREF(pFunc);
		Py_DECREF(pModule);
		Py_DECREF(driver_obj);
	}
}

int rasfly::imu::loadIMU(config_struct &raspi){
	int success = 0;
	imu_driver = raspi.imu_driver;
	if(imu_driver == PYTHON) {
		success = loadPython(raspi, raspi.imu_path); 	
	}
	return 0;
}

int rasfly::imu::loadPython(config_struct &raspi, const char *driver_name) {
	PyObject *pName;
	const char *func_name = "getState";
	const char *init_name = "init";

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

	state tmp_state;
	for(auto i=0; i<50; ++i) {
		// Let imu algorithms converge
		getState(tmp_state);
	}

	return 0;
}

int rasfly::imu::getState(state &rasfly_state) {
	if(imu_driver == PYTHON) {
		imu_struct imu_s;  
		PyObject *pValue;
		Py_buffer buf = {0};
		pValue = PyObject_CallObject(pFunc, driver_obj);	
		PyErr_Print();
		PyObject_GetBuffer(pValue, &buf, PyBUF_SIMPLE);	
		memcpy(&imu_s, buf.buf, sizeof(imu_struct));
		PyBuffer_Release(&buf);
		// Copy raw buffer into eigen 
		rasfly_state.euler(0) = imu_s.orientation.x;
		rasfly_state.euler(1) = imu_s.orientation.y;
		rasfly_state.euler(2) = imu_s.orientation.z;
		rasfly_state.accel(0) = imu_s.accel.x;
		rasfly_state.accel(1) = imu_s.accel.y;
		rasfly_state.accel(2) = imu_s.accel.z;
		rasfly_state.gyro(0) = imu_s.gyro.x;
		rasfly_state.gyro(1) = imu_s.gyro.y;
		rasfly_state.gyro(2) = imu_s.gyro.z;
	}
	return 0;
}