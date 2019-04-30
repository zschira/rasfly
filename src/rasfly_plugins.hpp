#ifndef _RASFLY_PLUGINS_H_
#define _RASFLY_PLUGINS_H_

#include <Python.h>

namespace rasfly {

class Plugins {
public:
	Plugins();
	~Plugins();	
private:
	PyObject *pClass, *pModule, *pInstance, *pDict;
	const char * driver_name, *class_name;
};

}

#endif