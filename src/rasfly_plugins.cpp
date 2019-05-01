#include "rasfly_plugins.hpp"
#include "json.hpp"

rasfly::Plugins::Plugins() : driver_name{"rasfly_py_api"}, class_name{"rasfly_api"},
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
	if((pModule = PyImport_Import(pName)) == NULL) {
		PyErr_Print();
		exit(0);
	}

	// Load api class
	pDict = PyModule_GetDict(pModule);
	pClass = PyDict_GetItemString(pDict, class_name);

	if(!PyCallable_Check(pClass)) {
		PyErr_Print();
		exit(0);
	}

	pInstance = PyObject_CallObject(pClass, NULL);
	Py_DecRef(pName);

	BindPlugins();
}

rasfly::Plugins::~Plugins() {
	Py_DecRef(pInstance);
	Py_DecRef(pDict);
	Py_DecRef(pClass);
	Py_DecRef(pModule);
	Py_Finalize();
}

void rasfly::Plugins::BindPlugins() {
	if((imu = PyObject_CallMethod(pInstance, "BindIMU", NULL)) != Py_None) {
		function_implemented["imu"] = true;
	}
}

State rasfly::Plugins::GetState() {
	State current;
	char *current_json;
	PyObject *pResult = PyObject_CallMethod(pInstance, "GetState", NULL);
	PyArg_Parse(pResult, "s", &current_json);
	auto j = nlohmann::json::parse(current_json);
	current.x = j["x"];
	Py_DecRef(pResult);
	return current;
}
