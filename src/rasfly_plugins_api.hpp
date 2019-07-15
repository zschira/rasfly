#ifndef _RASFLY_PLUGINS_API_H_
#define _RASFLY_PLUGINS_API_H_

#include <Python.h>
#include "rasfly_types.hpp"

//================================================================================
// Python api types
//================================================================================

//================================================================================
/// Vec3 Python type implementation
//================================================================================
//////////////////////////////////////////////////////////////////////////////////
/// @breif Function to access x, y, and z values from vec3
/// 
/// @param vec - Pointer to vec3 struct (self)
/// @param name - Name of member variable
/// @return value of component
//////////////////////////////////////////////////////////////////////////////////
static PyObject * Vec3_getattr(rasfly::Vec3 *vec, char *name) {
    PyObject *ret;
    if(strcmp(name, "x") == 0) {
        ret = PyFloat_FromDouble(vec->x);
    } else if(strcmp(name, "y") == 0) {
        ret = PyFloat_FromDouble(vec->y);
    } else if(strcmp(name, "z") == 0) {
        ret = PyFloat_FromDouble(vec->z);
    } else {
        PyErr_Format(PyExc_AttributeError, "'%.50s' object has no attribute '%.400s'", Py_TYPE(vec)->tp_name, name);
        return NULL;
    }
    return ret;
}

//////////////////////////////////////////////////////////////////////////////////
/// @breif Function to set x, y, and z values from vec3
/// 
/// @param vec - Pointer to vec3 struct (self)
/// @param name - Name of member variable
/// @param val - Value to set component to
/// @return 0 on success -1 on failure
//////////////////////////////////////////////////////////////////////////////////
static int Vec3_setattr(rasfly::Vec3 *vec, char *name, PyObject *val) {
    if(strcmp(name, "x") == 0) {
        vec->x = PyFloat_AsDouble(val);
    } else if(strcmp(name, "y") == 0) {
        vec->y = PyFloat_AsDouble(val);
    } else if(strcmp(name, "z") == 0) {
        vec->z = PyFloat_AsDouble(val);
    } else {
        PyErr_Format(PyExc_AttributeError, "'%.50s' object has no attribute '%.400s'", Py_TYPE(vec)->tp_name, name);
        return -1;
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////
/// @breif Function initialize new instance of Vec3 pytype
/// 
/// @param self - Pointer to vec3 struct (self). Allocated by new
/// @param args - Tuple that must contain x, y, and z values for new vec3
/// @param kwds - As of now keyword arguments are unused
/// @return 0 on success -1 on failure
//////////////////////////////////////////////////////////////////////////////////
static int vec3_init(PyObject *self, PyObject *args, PyObject *kwds) {
    double x,y,z;
    rasfly::Vec3 *obj = (rasfly::Vec3 *) self;
    PyArg_ParseTuple(args, "ddd", &x, &y, &z);
    obj->x = x;
    obj->y = y;
    obj->z = z;
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////
/// @breif Function allocate space for new instance of Vec3 pytype
/// 
/// @param type - PyTypeObject struct
/// @param args - Tuple that must contain x, y, and z values for new vec3
/// @param kwds - As of now keyword arguments are unused
/// @return 0 on success -1 on failure
//////////////////////////////////////////////////////////////////////////////////
PyObject *vec3_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    PyObject *obj = type->tp_alloc(type, 0);
    return obj;
}

//////////////////////////////////////////////////////////////////////////////////
/// @Struct Vec3 instance of PyTypeObject
//////////////////////////////////////////////////////////////////////////////////
static PyTypeObject vec3_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "rasfly.vec3",
    sizeof(rasfly::Vec3),
    0,
    0,                                          /* tp_dealloc */
    0,                                          /* tp_print */
    (getattrfunc)Vec3_getattr,                  /* tp_getattr */
    (setattrfunc)Vec3_setattr,                  /* tp_setattr */
    0,                                          /* tp_reserved */
    0,                                          /* tp_repr */
    0,                                          /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    0,                                          /* tp_hash */
    0,                                          /* tp_call */
    0,                                          /* tp_str */
    0,                                          /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    0,                                          /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    0,                                           /* tp_methods */
    0,                                          /* tp_members */
    0,                                           /* tp_getset */
    0,                                             /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    (initproc)vec3_init,                                /* tp_init */
    0,                                          /* tp_alloc */
    (newfunc)vec3_new,                                          /* tp_new */
};

//================================================================================
// State Python type implementation
//================================================================================
//////////////////////////////////////////////////////////////////////////////////
/// State Python type implementation
//////////////////////////////////////////////////////////////////////////////////
static PyObject *State_getattr(rasfly::State *state, char *name) {
    PyObject *obj = NULL;
    if(strcmp(name, "position") == 0) {
        obj = (PyObject *) &state->position;
    } else if(strcmp(name, "velocity") == 0) {
        obj = (PyObject *) &state->velocity;
    } else if(strcmp(name, "acceleration") == 0) {
        obj = (PyObject *) &state->acceleration;
    } else if(strcmp(name, "rotation") == 0) {
        obj = (PyObject *) &state->rotation;
    } else if(strcmp(name, "rotation_v") == 0) {
        obj = (PyObject *) &state->rotation_v;
    } else if(strcmp(name, "rotation_a") == 0) {
        obj = (PyObject *) &state->rotation_a;
    } else {
        PyErr_Format(PyExc_AttributeError, "'%.50s' object has no attribute '%.400s'", Py_TYPE(state)->tp_name, name);
        return NULL;
    }
    obj->ob_type = &vec3_type;
    return obj;
}

//////////////////////////////////////////////////////////////////////////////////
/// @breif Function to set State member variables (either scalar thrust or a vec3)
/// 
/// @param state - Pointer to state struct (self)
/// @param name - Name of member variable
/// @param vec - scalar thrust value or vec3
/// @return 0 on success or -1 on failure
//////////////////////////////////////////////////////////////////////////////////
static int State_setattr(rasfly::State *state, char *name, PyObject *vec) {
    // Set thrust member
    if(strcmp(name, "thrust") == 0) {
        state->thrust = PyFloat_AsDouble(vec);
        return 0;
    }

    // Set Vector members
    double x, y, z;
    if(PyArg_ParseTuple(vec, "ddd", &x, &y, &z) == 0) {
        PyErr_Print();
        exit(1);
    }
    if(strcmp(name, "position") == 0) {
        state->position = rasfly::Vec3(x, y, z);
    } else if(strcmp(name, "velocity") == 0) {
        state->velocity = rasfly::Vec3(x, y, z);
    } else if(strcmp(name, "acceleration") == 0) {
        state->acceleration = rasfly::Vec3(x, y, z);
    } else if(strcmp(name, "rotation") == 0) {
        state->rotation = rasfly::Vec3(x, y, z);
    } else if(strcmp(name, "rotation_v") == 0) {
        state->rotation_v = rasfly::Vec3(x, y, z);
    } else if(strcmp(name, "rotation_a") == 0) {
        state->rotation_a = rasfly::Vec3(x, y, z);
    } else {
        PyErr_Format(PyExc_AttributeError, "'%.50s' object has no attribute '%.400s'", Py_TYPE(state)->tp_name, name);
        return -1;
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////
/// @breif Function allocate space for new instance of State pytype
/// 
/// @param type - PyTypeObject struct
/// @param args - As of now args is unused
/// @param kwds - As of now keyword arguments are unused
/// @return 0 on success -1 on failure
//////////////////////////////////////////////////////////////////////////////////
static PyObject *state_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    PyObject *obj = type->tp_alloc(type, 0);
    return obj;
} 

//////////////////////////////////////////////////////////////////////////////////
/// @Struct State instance of PyTypeObject
//////////////////////////////////////////////////////////////////////////////////
static PyTypeObject state_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "rasfly.state",
    sizeof(rasfly::State),
    0,
    0,                                          /* tp_dealloc */
    0,                                          /* tp_print */
    (getattrfunc)State_getattr,                 /* tp_getattr */
    (setattrfunc)State_setattr,                 /* tp_setattr */
    0,                                          /* tp_reserved */
    0,                                          /* tp_repr */
    0,                                          /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    0,                                          /* tp_hash */
    0,                                          /* tp_call */
    0,                                          /* tp_str */
    0,                                          /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    0,                                          /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    0,                                          /* tp_methods */
    0,                                          /* tp_members */
    0,                                          /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                          /* tp_alloc */
    (newfunc)state_new,                                          /* tp_new */
};

//================================================================================
/// Thrust Python type implementation
//================================================================================
//////////////////////////////////////////////////////////////////////////////////
/// Thrust Python type implementation
//////////////////////////////////////////////////////////////////////////////////
static PyObject *Thrust_getattr(rasfly::Thrust *thrust, char *name) {
    PyObject *ret;
    if(strcmp(name, "T1") == 0) {
        ret = PyFloat_FromDouble(thrust->T1);
    } else if(strcmp(name, "T2") == 0) {
        ret = PyFloat_FromDouble(thrust->T2);
    } else if(strcmp(name, "T3") == 0) {
        ret = PyFloat_FromDouble(thrust->T3);
    } else if(strcmp(name, "T4") == 0) {
        ret = PyFloat_FromDouble(thrust->T4);
    } else {
        PyErr_Format(PyExc_AttributeError, "'%.50s' object has no attribute '%.400s'", Py_TYPE(thrust)->tp_name, name);
        return NULL;
    }
    return ret;
}

//////////////////////////////////////////////////////////////////////////////////
/// @breif Function to set Thrust member variables (either scalar thrust or a vec3)
/// 
/// @param thrust - Pointer to thrust struct (self)
/// @param name - Name of member variable
/// @param vec - scalar thrust value or vec3
/// @return 0 on success or -1 on failure
//////////////////////////////////////////////////////////////////////////////////
static int Thrust_setattr(rasfly::Thrust *thrust, char *name, PyObject *val) {
    if(strcmp(name, "T1") == 0) {
        thrust->T1 = PyFloat_AsDouble(val);
    } else if(strcmp(name, "T2") == 0) {
        thrust->T2 = PyFloat_AsDouble(val);
    } else if(strcmp(name, "T3") == 0) {
        thrust->T3 = PyFloat_AsDouble(val);
    } else if(strcmp(name, "T4") == 0) {
        thrust->T4 = PyFloat_AsDouble(val);
    } else {
        PyErr_Format(PyExc_AttributeError, "'%.50s' object has no attribute '%.400s'", Py_TYPE(val)->tp_name, name);
        return -1;
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////
/// @breif Function allocate space for new instance of Thrust pytype
/// 
/// @param type - PyTypeObject struct
/// @param args - As of now args is unused
/// @param kwds - As of now keyword arguments are unused
/// @return 0 on success -1 on failure
//////////////////////////////////////////////////////////////////////////////////
static PyObject *thrust_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    PyObject *obj = type->tp_alloc(type, 0);
    return obj;
} 

//////////////////////////////////////////////////////////////////////////////////
/// @Struct Thrust instance of PyTypeObject
//////////////////////////////////////////////////////////////////////////////////
static PyTypeObject thrust_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "rasfly.state",
    sizeof(rasfly::Thrust),
    0,
    0,                                          /* tp_dealloc */
    0,                                          /* tp_print */
    (getattrfunc)Thrust_getattr,                 /* tp_getattr */
    (setattrfunc)Thrust_setattr,                 /* tp_setattr */
    0,                                          /* tp_reserved */
    0,                                          /* tp_repr */
    0,                                          /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    0,                                          /* tp_hash */
    0,                                          /* tp_call */
    0,                                          /* tp_str */
    0,                                          /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    0,                                          /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    0,                                          /* tp_methods */
    0,                                          /* tp_members */
    0,                                          /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                          /* tp_alloc */
    (newfunc)thrust_new,                                          /* tp_new */
};

//================================================================================
// Python api initialization
//================================================================================

//////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize pytypes
//////////////////////////////////////////////////////////////////////////////////
static int rasfly_exec(PyObject *mod) {
    if(PyType_Ready(&vec3_type) < 0) {
        return -1;
    }

    if(PyType_Ready(&state_type) < 0) {
        return -1;
    }

    if(PyType_Ready(&thrust_type) < 0) {
        return -1;
    }

    Py_INCREF(&vec3_type);
    if(PyModule_AddObject(mod, "vec3", (PyObject *) &vec3_type) < 0) {
        return -1;
    }

    Py_INCREF(&state_type);
    if(PyModule_AddObject(mod, "state", (PyObject *) &state_type) < 0) {
        return -1;
    }

    Py_INCREF(&thrust_type);
    if(PyModule_AddObject(mod, "thrust", (PyObject *) &thrust_type) < 0) {
        return -1;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////
/// @array Module slot array
//////////////////////////////////////////////////////////////////////////////////
static PyModuleDef_Slot rasfly_slots[] = {
    {Py_mod_exec, (void *) rasfly_exec},
    {0, NULL},
};

//////////////////////////////////////////////////////////////////////////////////
/// @brief Create module
//////////////////////////////////////////////////////////////////////////////////
static PyModuleDef api_Module = {
    PyModuleDef_HEAD_INIT,
    "rasfly",
    "rasfly api",
    0,
    NULL,
    rasfly_slots,
    NULL,
    NULL,
    NULL,
};

//////////////////////////////////////////////////////////////////////////////////
/// @brief Function to initialize module
//////////////////////////////////////////////////////////////////////////////////
PyMODINIT_FUNC PyInit_rasfly(void) {
    return PyModuleDef_Init(&api_Module);
}

#endif