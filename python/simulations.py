from scipy import integrate
from math import sin, cos, radians
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def euler2mat(phi, theta, psi):
    R = np.array([[cos(theta)*cos(psi), sin(phi)*sin(theta)*cos(psi)-cos(phi)*sin(psi), cos(phi)*sin(theta)*cos(psi)+sin(phi)*sin(psi)],
                  [cos(theta)*sin(psi), sin(phi)*sin(theta)*sin(psi)+cos(phi)*cos(psi), cos(phi)*sin(theta)*sin(psi)-sin(phi)*cos(psi)],
                  [-sin(theta), sin(phi)*cos(theta), cos(phi)*cos(theta)]])

    return R

def getStateMat():
    return np.array([[1, 1, -1, -1],
                  [1, -1, -1, 1],
                  [1, -1, 1, -1],
                  [1, 1, 1, 1]])

def quadcopter(y, t, params):
    m = params.mass;      #[kg]
    Rr = params.Rr;     # [m^2]
    Ix = params.Ix;     # [m/s]
    Iy = params.Iy;     # [m/s]
    Iz = params.Iz;     # [m/s]
    g = params.g;      # gravitational acceleration [m/s^2]
    k = params.k
    Cd = params.Cd
    force = params.thrust
    Kd = params.Kd
    Kp = params.Kp
    trim = params.trim

    A = getStateMat()

    u = y[3]; v = y[4]; w = y[5]
    phi = y[6]; theta = y[7]; psi = y[8]
    wx = y[9]; wy = y[10]; wz = y[11]

    R = euler2mat(phi, theta, psi)

    angles = np.matmul(R, np.array([wx, wy, wz]).T)

    b = np.array([[Ix/Rr * (Kd*(trim[9] - wx) + Kp*(trim[6] - phi))],
                  [Iy/Rr * (Kd*(trim[10] - wy) + Kp*(trim[7] - theta))],
                  [-k*(Iz * Kd * (trim[11] - wz))],
                  [force]])

    thrusts = np.linalg.solve(A, b)

    thrust = np.array([0, 0, np.sum(thrusts)]).T
    thrust = np.matmul(R, thrust)
    gravity = np.array([0, 0, -m*g]).T
    drag = np.array([-u, -v, -w]).T * Cd

    accel = (gravity + thrust + drag)/m
    
    dydt = np.array([u,
                     v,
                     w,
                     accel[0],
                     accel[1],
                     accel[2],
                     angles[0],
                     angles[1],
                     angles[2],
                     (Iy - Iz)/Ix * wy*wz + Rr/Ix * (thrusts[0, 0] + thrusts[1, 0] - thrusts[2, 0] - thrusts[3, 0]),
                     (Iz - Ix)/Iy * wx*wz + Rr/Iy * (thrusts[0, 0] - thrusts[1, 0] - thrusts[2, 0] + thrusts[3, 0]),
                     (Ix - Iy)/Iz * wy*wx + k/Iz * (thrusts[1, 0] + thrusts[3, 0] - thrusts[0, 0] - thrusts[2, 0])])

    return dydt

class params_class():
    def __init__(self):
        self.mass = 1            # vehicle mass in [kg];
        self.Rr = 0.2         # Radius out to rotors from COM, [m]
        self.Ix = 2e-3      # Moment of Inertia about x axis, [kg*m^2]
        self.Iy = self.Ix
        self.Iz = 4e-3     # Moment of Inertia about z axis, [kg*m^2]
        self.g = 9.81;        # gravitational acceleration
        self.k = 0.1
        self.Cd = 1e-4
        self.Kd = 10
        self.Kp = 50
        self.thrust = self.mass * self.g
        self.trim = [0, 0, 0, 0, 0, 0, radians(50), radians(50), 0, 0, 0, radians(10)]

params = params_class()
y = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
t = np.linspace(0, 10, 1000)
sol = integrate.odeint(quadcopter, y, t, args=(params,))

# back out forces
A = getStateMat()
trim = params.trim
thrusts = np.zeros((sol.shape[0], 4))
counter = 0
for state in sol:
    m = params.mass;      #[kg]
    Rr = params.Rr;     # [m^2]
    Ix = params.Ix;     # [m/s]
    Iy = params.Iy;     # [m/s]
    Iz = params.Iz;     # [m/s]
    g = params.g;      # gravitational acceleration [m/s^2]
    k = params.k
    Cd = params.Cd
    force = params.thrust
    Kd = params.Kd
    Kp = params.Kp
    trim = params.trim

    u = state[3]; v = state[4]; w = state[5]
    phi = state[6]; theta = state[7]; psi = state[8]
    wx = state[9]; wy = state[10]; wz = state[11]

    b = np.array([[Ix/Rr * (Kd*(trim[9] - wx) + Kp*(trim[6] - phi))],
                  [Iy/Rr * (Kd*(trim[10] - wy) + Kp*(trim[7] - theta))],
                  [k*(Iz * Kd * (trim[11] - wz))],
                  [force]])

    t_tmp = np.linalg.solve(A, b)
    thrusts[counter, :] = t_tmp[:, 0]
    counter += 1


minim = sol[:, :3].min()
maxim = sol[:, :3].max()


fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.set_xlim3d(minim, maxim)
ax.set_ylim3d(minim, maxim)
ax.set_zlim3d(minim, maxim)
ax.plot(sol[:,0], sol[:,1], sol[:,2])

fig2 = plt.figure()
ax2 = fig2.add_subplot(111)

ax2.plot(t, sol[:, 6])
ax2.plot(t, sol[:, 7])
ax2.plot(t, sol[:, 8])
ax2.legend(["theta", "phi", "psi"])

fig2 = plt.figure()
ax2 = fig2.add_subplot(111)

ax2.plot(t, sol[:, 9])
ax2.plot(t, sol[:, 10])
ax2.plot(t, sol[:, 11])
ax2.legend(["p", "q", "r"])

fig3 = plt.figure()
ax3 = fig3.add_subplot(111)
ax3.plot(t, thrusts[:, 0])
ax3.plot(t, thrusts[:, 1])
ax3.plot(t, thrusts[:, 2])
ax3.plot(t, thrusts[:, 3])
ax3.legend(["T1", "T2", "T3", "T4"])

plt.show()