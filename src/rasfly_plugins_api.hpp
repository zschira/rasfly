#ifndef _RASFLY_PLUGINS_API_H_
#define _RASFLY_PLUGINS_API_H_

#include <Python.h>
#include "rasfly_types.hpp"

rasfly::State *py_state;
rasfly::Thrust_4M *py_thrust;
rasfly::State *py_trim;
double *py_thrust_mag;

PyObject * set_Vec3(PyObject *self, PyObject *args, rasfly::Vec3 &vec) {
	if(!PyArg_ParseTuple(args, "ddd", &vec.x, &vec.y, &vec.z)) {
		PyErr_Print();
	}
	PyObject * succes = PyLong_FromLong(0);
	return succes;
}

PyObject * get_Vec3(PyObject *self, PyObject *args, rasfly::Vec3 &vec) {
	PyObject *tuple = PyTuple_New(3);
	PyTuple_SetItem(tuple, 0, PyFloat_FromDouble(vec.x));
	PyTuple_SetItem(tuple, 1, PyFloat_FromDouble(vec.y));
	PyTuple_SetItem(tuple, 2, PyFloat_FromDouble(vec.z));
	return tuple;
}

auto set_position = [](PyObject *self, PyObject *args) {return set_Vec3(self, args, py_state->position);};
auto set_rotation = [](PyObject *self, PyObject *args) {return set_Vec3(self, args, py_state->rotation);};
auto get_position = [](PyObject *self, PyObject *args) {return get_Vec3(self, args, py_state->position);};
auto get_rotation = [](PyObject *self, PyObject *args) {return get_Vec3(self, args, py_state->rotation);};

PyMethodDef api_Methods[] = {
	{"set_position", set_position, METH_VARARGS, "Set position vector"},
	{"set_rotation", set_rotation, METH_VARARGS, "Set euler angles"},
	{"get_position", get_position, METH_NOARGS, "Get position vector"},
	{"get_rotation", get_rotation, METH_NOARGS, "Get rotation vector"},
	{NULL, NULL, 0, NULL}
};

PyModuleDef api_Module = {
	PyModuleDef_HEAD_INIT, "rasfly", "rasfly api", -1, api_Methods, NULL, NULL, NULL, NULL
};

PyObject * PyInit_api(void) {
	return PyModule_Create(&api_Module);
}

void InitAPI(rasfly::State *api_state, rasfly::Thrust_4M *api_thrust, rasfly::PilotInput *api_input) {
	py_state = api_state;
	py_thrust = api_thrust;
	py_trim = &api_input->trim;
	py_thrust_mag = &api_input->thrust;
}

#endif