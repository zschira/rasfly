#ifndef _RASFLY_PLUGINS_API_H_
#define _RASFLY_PLUGINS_API_H_

#include <Python.h>
#include "rasfly_types.hpp"

rasfly::State *py_state;
rasfly::Thrust_4M *py_thrust;

PyObject * set_Vec3(PyObject *self, PyObject *args, rasfly::Vec3 &vec) {
	if(!PyArg_ParseTuple(args, "ddd", &vec.x, &vec.y, &vec.z)) {
		PyErr_Print();
	}
	PyObject * succes = PyLong_FromLong(0);
	return succes;
}

auto set_position = [](PyObject *self, PyObject *args) {return set_Vec3(self, args, py_state->position);};
auto set_rotation = [](PyObject *self, PyObject *args) {return set_Vec3(self, args, py_state->rotation);};

PyMethodDef api_Methods[] = {
	{"set_position", set_position, METH_VARARGS, "Set position vector"},
	{"set_rotation", set_rotation, METH_VARARGS, "Set euler angles"},
	{NULL, NULL, 0, NULL}
};

PyModuleDef api_Module = {
	PyModuleDef_HEAD_INIT, "rasfly", "rasfly api", -1, api_Methods, NULL, NULL, NULL, NULL
};

PyObject * PyInit_api(void) {
	return PyModule_Create(&api_Module);
}

void InitState(rasfly::State *api_state) {
	py_state = api_state;
}

void InitThrust(rasfly::Thrust_4M *api_thrust) {
	py_thrust = api_thrust;
}

#endif