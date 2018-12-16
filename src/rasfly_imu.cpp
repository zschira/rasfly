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

	PyObject *pArgs, *pValue, *pInit;
	pInit = PyObject_GetAttrString(pModule, init_name);	
	if(!pInit || !PyCallable_Check(pInit)) {
		PyErr_Print();
		return 1;
	}
	driver_obj = PyTuple_New(1);
	pValue = PyObject_CallObject(pInit, pArgs);
	PyTuple_SetItem(driver_obj, 0, pValue);

	return 0;
}

int rasfly::imu::getState(state &rasfly_state) {
	if(imu_driver == PYTHON) {
		PyObject *pValue;
		Py_buffer buf = {0};
		pValue = PyObject_CallObject(pFunc, driver_obj);	
		PyErr_Print();
		PyObject_GetBuffer(pValue, &buf, PyBUF_SIMPLE);	
		memcpy(&rasfly_state, buf.buf, sizeof(state));
		PyBuffer_Release(&buf);
		printf("%f\n", rasfly_state.orientation.y);
	}
	return 0;
}