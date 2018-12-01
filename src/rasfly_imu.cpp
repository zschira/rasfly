#include "rasfly_imu.h"
#include <Python.h>
#include <iostream>

int rasfly::loadIMU(hardware &raspi, const char *driver_name, driver_types driver){
	int success = 0;
	if(driver == PYTHON) {
		success = loadPython(raspi, driver_name, driver); 	
	}
	return 0;
}

int rasfly::loadPython(hardware &raspi, const char *driver_name, driver_types driver) {
	PyObject *pName, *pModule, *pFunc;
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