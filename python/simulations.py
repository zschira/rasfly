from scipy import integrate
from math import sin, cos, radians
import numpy as np
import matplotlib.pyplot as plt

def euler2mat(theta, phi, psi):
	R = np.array([[cos(theta)*cos(psi), sin(phi)*sin(theta)*cos(psi)-cos(phi)*sin(psi), cos(phi)*sin(theta)*cos(psi)+sin(phi)*sin(psi)],
				  [cos(theta)*sin(psi), sin(phi)*sin(theta)*sin(psi)+cos(phi)*cos(psi), cos(phi)*sin(theta)*sin(psi)-sin(phi)*cos(psi)],
				  [-sin(theta), sin(phi)*cos(theta), cos(phi)*cos(theta)]])

	return R


def quadcopter(y, t, params):
	m = params.mass;      #[kg]
	Rr = params.Rr;     # [m^2]
	Ix = params.Ix;     # [m/s]
	Iy = params.Iy;     # [m/s]
	Iz = params.Iz;     # [m/s]
	g = params.g;      # gravitational acceleration [m/s^2]
	k = params.k
	kd = params.kd

	u = y[3]; v = y[4]; w = y[5]
	theta = y[6]; phi = y[7]; psi = y[8]
	wx = y[9]; wy = y[10]; wz = y[11]

	R = euler2mat(theta, phi, psi)

	angles = np.matmul(R, np.array([wx, wy, wz]).T)

	thrusts = np.ones((4)) * 1/4 * m * g

	thrust = np.ones((3)) * np.sum(thrusts)
	thrust = np.matmul(R, thrust)
	gravity = np.array([0, 0, -m*g]).T
	drag = np.array([-u, -v, -w]).T * kd

	accel = (gravity + thrust + drag)
	
	dydt = np.array([u,
					 v,
					 w,
					 accel[0],
					 accel[1],
					 accel[2],
					 angles[0],
					 angles[1],
					 angles[2],
					 (Iy - Iz)/Ix * wy*wz + Rr/Ix * (thrusts[1] - thrusts[3]),
					 (Iz - Ix)/Iy * wx*wz + Rr/Iy * (thrusts[0] - thrusts[2]),
					 (Ix - Iy)/Iz * wy*wx + k/Iz * (thrusts[1] + thrusts[3] - thrusts[0] - thrusts[2])])

	return dydt

class params_class():
	def __init__(self):
		self.mass = 1;            # vehicle mass in [kg];
		self.Rr = 0.2;         # Radius out to rotors from COM, [m]
		self.Ix = 2e-3	  # Moment of Inertia about x axis, [kg*m^2]
		self.Iy = self.Ix;
		self.Iz = 4e-3     # Moment of Inertia about z axis, [kg*m^2]
		self.g = 9.81;        # gravitational acceleration
		self.k = 0.1
		self.kd = 1e-4

params = params_class()
y = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
t = np.linspace(0, 10, 101)
sol = integrate.odeint(quadcopter, y, t, args=(params,))

plt.plot(t, sol[:, 2])
plt.show()