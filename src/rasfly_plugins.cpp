#include "rasfly_plugins.hpp"

rasfly::Plugins::Plugins() : driver_name{"rasfly_py_api"}, class_name{"rasfly_api"} {
	PyObject * pName;

	// Initialize python interpreter
	Py_Initialize();

	// Set import path
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("import os");
	PyRun_SimpleString("sys.path.append(os.getcwd())");

	// Import module
	pName = PyUnicode_DecodeFSDefault(driver_name);
	if((pModule = PyImport_Import(pName)) == NULL) {
		PyErr_Print();
		exit(0);
	}

	pDict = PyModule_GetDict(pModule);
	pClass = PyDict_GetItemString(pDict, class_name);

	if(!PyCallable_Check(pClass)) {
		PyErr_Print();
		exit(0);
	}

	pInstance = PyObject_CallObject(pClass, NULL);
	Py_DecRef(pName);
}

rasfly::Plugins::~Plugins() {
	Py_DecRef(pInstance);
	Py_DecRef(pDict);
	Py_DecRef(pClass);
	Py_DecRef(pModule);
	Py_Finalize();
}