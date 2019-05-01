#ifndef _RASFLY_PLUGINS_H_
#define _RASFLY_PLUGINS_H_

#include <Python.h>
#include "rasfly_types.hpp"
#include <map>
#include <string>

namespace rasfly {

class Plugins {
public:
	Plugins();
	~Plugins();	
	State GetState();
private:
	void BindPlugins();
	PyObject *pClass, *pModule, *pInstance, *pDict;
	PyObject *imu, *filter, *controller, *esc;
	std::map<std::string, bool> function_implemented;
	const char *driver_name, *class_name;
};

}

#endif