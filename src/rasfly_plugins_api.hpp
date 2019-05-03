#ifndef _RASFLY_PLUGINS_API_H_
#define _RASFLY_PLUGINS_API_H_

#include <Python.h>
#include "rasfly_types.hpp"

static double *x = NULL;
static double *y = NULL;
static double *z = NULL;
static double *qw = NULL;
static double *qx = NULL;
static double *qy = NULL;
static double *qz = NULL;
static double *t1 = NULL;
static double *t2 = NULL;
static double *t3 = NULL;
static double *t4 = NULL;

static PyObject * set_position(PyObject * self, PyObject * args) {
	auto status = PyArg_ParseTuple(args, "ddd", x, y, z);
	if(!status) {
		PyErr_Print();
	}
	PyObject * succes = PyLong_FromLong(0);
	return succes;
}

static PyObject * set_quaternion(PyObject * self, PyObject * args) {
	auto status = PyArg_ParseTuple(args, "dddd", qw, qx, qy, qz);
	if(!status) {
		PyErr_Print();
	}
	PyObject * succes = PyLong_FromLong(0);
	return succes;
}

static PyMethodDef api_Methods[] = {
	{"set_position", set_position, METH_VARARGS, "Set position vector"},
	{NULL, NULL, 0, NULL}
};

static PyModuleDef api_Module = {
	PyModuleDef_HEAD_INIT, "rasfly", "rasfly api", -1, api_Methods, NULL, NULL, NULL, NULL
};

static PyObject * PyInit_api(void) {
	return PyModule_Create(&api_Module);
}

static void InitState(rasfly::State &api_state) {
	x = &api_state.x;
	y = &api_state.y;
	z = &api_state.z;
	qw = &api_state.quaternion.w;
	qx = &api_state.quaternion.x;
	qy = &api_state.quaternion.y;
	qz = &api_state.quaternion.z;
}

static void InitThrust(rasfly::Thrust_4M &api_thrust) {
	t1 = &api_thrust.T1;
	t2 = &api_thrust.T2;
	t3 = &api_thrust.T3;
	t4 = &api_thrust.T4;	
}

#endif