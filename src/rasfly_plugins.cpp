#include "rasfly_plugins.hpp"
#include "rasfly_plugins_api.hpp"
#include "json.hpp"
#include "log.hpp"

rasfly::Plugins::Plugins() : driver_name{"rasfly_py_api"}, class_name{"rasfly_api"}, pobjs(std::make_unique<PyObjs>()),
	function_implemented{
		{"imu", false}, 
		{"filter", false}, 
		{"inputs", false}, 
		{"controller", false}, 
		{"motors", false} 
	}
{
	pobjs->py_type_map = {
		{"vec3", &vec3_type},
		{"state", &state_type},
		{"thrust", &thrust_type}
	};

	PyObject *module, *dict, *type;
	// Create python api module
	PyImport_AppendInittab("rasfly", &PyInit_rasfly);

	// Initialize python interpreter
	Py_Initialize();

	// Set import path
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("import os");
	PyRun_SimpleString("sys.path.append(os.getcwd())");

	// Import api module
	if((module = PyImport_ImportModule(driver_name)) == NULL) {
		PyErr_Print();
		exit(0);
	}

	// Load api class
	dict = PyModule_GetDict(module);
	type = PyDict_GetItemString(dict, class_name);

	// Load api class
	if(!PyCallable_Check(type)) {
		PyErr_Print();
		exit(0);
	}

	Py_DecRef(module);
	Py_DecRef(dict);
	Py_DecRef(type);

	pobjs->apiInstance = PyObject_CallObject(type, NULL);

	BindPlugins();
}

rasfly::Plugins::~Plugins() {
	for(auto const& func : pobjs->func_map) {
		Py_DecRef(func.second);
	}
	Py_DecRef(pobjs->apiInstance);
	Py_Finalize();
}

void rasfly::Plugins::BindPlugins() {
	for(auto &func : function_implemented) {
		PyObject * result = PyObject_CallMethod(pobjs->apiInstance, "BindFunc", "(s)", func.first);

		if(result != Py_None && PyCallable_Check(result)) {
			func.second = true;
			pobjs->func_map[func.first] = result;
		}
	}
}

bool rasfly::Plugins::IsImplemented(const char *function) {
	return function_implemented[function];
}
