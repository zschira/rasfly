#include "rasfly_plugins.hpp"
#include "rasfly_plugins_api.hpp"
#include "json.hpp"
#include <Python.h>

struct rasfly::Plugins::PyObjs {
	PyObject *pClass, *pModule, *pInstance, *pDict;
	std::map<const char *, PyObject *> func_map;
};

rasfly::Plugins::Plugins() : driver_name{"rasfly_py_api"}, class_name{"rasfly_api"}, pobjs(std::make_unique<PyObjs>()),
	function_implemented{
		{"imu", false}, 
		{"filter", false}, 
		{"inputs", false}, 
		{"controller", false}, 
		{"motors", false} 
	}
{
	// Create python api module
	InitState(&api_state);
	InitThrust(&api_thrust);
	PyImport_AppendInittab("rasfly", &PyInit_api);

	// Initialize python interpreter
	Py_Initialize();

	// Set import path
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("import os");
	PyRun_SimpleString("sys.path.append(os.getcwd())");

	// Import module
	PyObject *pName = PyUnicode_DecodeFSDefault(driver_name);
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
	for(auto const& func : pobjs->func_map) {
		Py_DecRef(func.second);
	}
	Py_Finalize();
}

void rasfly::Plugins::BindPlugins() {
	for(auto &func : function_implemented) {
		PyObject * result = PyObject_CallMethod(pobjs->pInstance, "BindFunc", "(s)", func.first);

		if(result != Py_None && PyCallable_Check(result)) {
			func.second = true;
			pobjs->func_map[func.first] = result;
		}
	}
}

void rasfly::Plugins::Execute(const char *function) {
	PyObject_CallObject(pobjs->func_map[function], NULL);
	PyErr_Print();
}

bool rasfly::Plugins::IsImplemented(const char *function) {
	return function_implemented[function];
}
