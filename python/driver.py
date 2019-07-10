from rasfly_py_api import *
from Adafruit_BNO055 import BNO055    
from math import radians

bno = BNO055.BNO055(rst=None, busnum=1)
if not bno.begin():
    raise RuntimeError('Failed to initialize BNO055! Is the sensor connected?')


def getState():
    euler = vec3()
    accel = vec3()
    gyro = vec3()
    euler.z, euler.x, euler.y = bno.read_euler()
    euler.x = radians(euler.x); euler.y = radians(euler.y); euler.z = radians(euler.z)
    accel.x, accel.y, accel.z =bno.read_linear_acceleration()
    gyro.x, gyro.y, gyro.z = bno.read_gyroscope()
    gyro.x = radians(gyro.x); gyro.y = radians(gyro.y); gyro.z = radians(gyro.z)
    curr = state(euler, accel, gyro)
    return curr
