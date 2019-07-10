#ifndef _RASFLY_PLUGINS_H_
#define _RASFLY_PLUGINS_H_

#include "rasfly_types.hpp"
#include <map>
#include <memory>
#include <Python.h>
#include <functional>

namespace rasfly {

class Plugins {
public:
	Plugins();
	~Plugins();	
	template<typename T, typename... Arguments>
	T Execute(const char *function, Arguments*... parameters) {
		PyObject *args = NULL;
		if constexpr(!std::is_same<T, void>::value) {
			T result; 
		}
		if constexpr (sizeof...(parameters) > 0) {
			args = PyTuple_Pack(sizeof...(Arguments), (...,test(parameters)));
		}
		PyObject *ret = PyObject_CallObject(pobjs->func_map[function], args);
		Py_DecRef(args);
		if constexpr(!std::is_same<T, void>::value) {
			T type(*((T *) ret));
			return type;
		}
	}

	bool IsImplemented(const char *function);
private:
	void BindPlugins();
	template<typename T>
	PyObject *test(T param) {
		PyObject *obj = (PyObject *) param;
		obj->ob_type = pobjs->py_type_map[param->name];
		obj->ob_refcnt = 1;
		return obj;
	}
	struct PyObjs {
		PyObject *apiInstance;
		std::map<const char *, PyObject *> func_map;
		std::map<const char *, PyTypeObject *> py_type_map;
	};
	std::unique_ptr<PyObjs> pobjs;
	std::map<const char *, bool> function_implemented;
	const char *driver_name, *class_name;
};

}

#endif