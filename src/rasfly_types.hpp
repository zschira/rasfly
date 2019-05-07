#ifndef _RASFLY_TYPES_H_
#define _RASFLY_TYPES_H_

#include "json.hpp"
#include <string>
#include <array>

namespace rasfly {

enum class Unit { DISTANCE, VELOCITY, ACCELERATION, ANGLE, ANGLE_RATE, ANGLE_ACCEL };

struct Vec3 {
	Vec3() { values.fill(0.0); }
	Vec3(double _x, double _y, double _z) { x = _x; y = _y; z = _z; }
	std::array<double, 3> values;
	double &x = values[0];
	double &y = values[1];
	double &z = values[2];
	
	inline friend Vec3 operator-(const Vec3& lhs, const Vec3& rhs) {
		return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	template<class mat>
	inline friend mat& operator<< (mat& lhs, const Vec3& rhs) {
		lhs << rhs.x, rhs.y, rhs.z;
		return lhs;
	}
};

struct State {
	State() {}
	// Position
	Vec3 position;
	Vec3 velocity;
	Vec3 acceleration;
	Vec3 rotation;
	Vec3 rotation_v;
	Vec3 rotation_a;
};

struct Gains {
	Gains(const nlohmann::json &json_str, std::string axis) {
		kp = json_str[axis + "_proportional_gain"];
		kd = json_str[axis + "_derivative_gain"];
		ki = json_str[axis + "_integral_gain"];
	}
	double kp;				// Proportional gains
	double kd;				// Derivative gains
	double ki;				// Integral gains
};

struct MomentsInertia {		// [kg*m^2]
	MomentsInertia(const nlohmann::json &json_str) {
		Ix = json_str["Ix"];
		Iy = json_str["Iy"];
		Iz = json_str["Iz"];
	}
	double Ix;	
	double Iy;	
	double Iz;	
};

struct TorqueContribution {
	TorqueContribution(const nlohmann::json &json_str, const std::string &axis) {
		if(json_str["frame_type"] == "x") {
			if(axis == "X") {
				M1 = 1; M2 = 1; M3 = -1; M4 = -1;
			} else if(axis == "Y") {
				M1 = 1; M2 = -1; M3 = -1; M4 = 1;
			} else if(axis == "Z") {
				M1 = 1; M2 = -1; M3 = 1; M4 = -1;
			}
		}
	}
	// Denotes whether each motor contributes to a torque
	double M1;
	double M2;
	double M3;
	double M4;
};

struct PhysicalProperties {
	PhysicalProperties(const nlohmann::json &json_str) : 
	roll(json_str, "roll"), pitch(json_str, "pitch"), 
	yaw(json_str, "yaw"), moments(json_str),
	taux(json_str, "X"), tauy(json_str, "Y"), tauz(json_str, "Z") 
	{
		mass = json_str["mass"];
		motor_radius = json_str["motor_radius"];
		max_thrust = json_str["max_thrust"];
	}
	double mass;			// [kg]
	double motor_radius;	// [m]
	double max_thrust;		// [N]
	Gains roll;
	Gains pitch;
	Gains yaw;
	MomentsInertia moments;
	TorqueContribution taux;
	TorqueContribution tauy;
	TorqueContribution tauz;
};

struct Thrust_4M {
	Thrust_4M(const double _T1, const double _T2, const double _T3, const double _T4) {
		T1 = _T1;
		T2 = _T2;
		T3 = _T3;
		T4 = _T4;
	}
	Thrust_4M() { T1 = 0.0; T2 = 0.0; T3 = 0.0; T4 = 0.0; }
	double T1;
	double T2;
	double T3;
	double T4;
};

}

#endif