#include "rasfly_plugins.hpp"
#include "json.hpp"
#include <Python.h>

struct rasfly::Plugins::PyObjs {
	PyObject *pClass, *pModule, *pInstance, *pDict;
};

rasfly::Plugins::Plugins() : driver_name{"rasfly_py_api"}, class_name{"rasfly_api"}, pobjs(std::make_unique<PyObjs>()),
	function_implemented{
		{"imu", false}, 
		{"filter", false}, 
		{"controller", false}, 
		{"esc", false} 
	}
{
	PyObject * pName;

	// Initialize python interpreter
	Py_Initialize();

	// Set import path
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("import os");
	PyRun_SimpleString("sys.path.append(os.getcwd())");

	// Import module
	pName = PyUnicode_DecodeFSDefault(driver_name);
	if((pobjs->pModule = PyImport_Import(pName)) == NULL) {
		PyErr_Print();
		exit(0);
	}

	// Load api class
	pobjs->pDict = PyModule_GetDict(pobjs->pModule);
	pobjs->pClass = PyDict_GetItemString(pobjs->pDict, class_name);

	if(!PyCallable_Check(pobjs->pClass)) {
		PyErr_Print();
		exit(0);
	}

	pobjs->pInstance = PyObject_CallObject(pobjs->pClass, NULL);
	Py_DecRef(pName);

	BindPlugins();
}

rasfly::Plugins::~Plugins() {
	Py_DecRef(pobjs->pInstance);
	Py_DecRef(pobjs->pDict);
	Py_DecRef(pobjs->pClass);
	Py_DecRef(pobjs->pModule);
	Py_Finalize();
}

void rasfly::Plugins::BindPlugins() {
	PyObject *result;

	if((result = PyObject_CallMethod(pobjs->pInstance, "BindFunc", "(s)", "imu")) != Py_None) {
		function_implemented["imu"] = true;
	}
}

State rasfly::Plugins::GetState() {
	char *current_json;
	PyObject *pResult = PyObject_CallMethod(pobjs->pInstance, "GetState", NULL);
	PyArg_Parse(pResult, "s", &current_json);
	auto j = nlohmann::json::parse(current_json);
	State current(j);
	Py_DecRef(pResult);
	return current;
}

Thrust rasfly::Plugins::CalcThrust() {
	Thrust thust;
	return thust;
}

bool rasfly::Plugins::IsImplemented(const char *function) {
	return function_implemented[function];
}
