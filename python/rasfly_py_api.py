import ctypes

class quaternion(ctypes.Structure):
	"""docstring for quaternion"""
	_fields_ = [("w", ctypes.c_double),
			     ("x", ctypes.c_double),
			     ("y", ctypes.c_double),
			     ("z", ctypes.c_double)]

class vec3(ctypes.Structure):
	"""docstring for quaternion"""
	_fields_ = [("x", ctypes.c_double),
			     ("y", ctypes.c_double),
			     ("z", ctypes.c_double)]

class state(ctypes.Structure):
	"""docstring for quaternion"""
	_fields_ = [("orientation", quaternion),
			     ("accel", vec3),
			     ("gyro", vec3)]