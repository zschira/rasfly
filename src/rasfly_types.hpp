#ifndef _RASFLY_TYPES_H_
#define _RASFLY_TYPES_H_

#include "json.hpp"
#include <string>

namespace rasfly {

struct Quaternion {
	double w;
	double x;
	double y;
	double z;
};

struct State {
	State() {}
	State(const nlohmann::json &json_str) {
		x = json_str["x"];
		y = json_str["y"];
		z = json_str["z"];
		quaternion.w = json_str["qw"];
		quaternion.x = json_str["qx"];
		quaternion.y = json_str["qy"];
		quaternion.z = json_str["qz"];
	}
	// Position
	double x;
	double y;
	double z;
	Quaternion quaternion;
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
		// Denotes whether each motor contributes to a torque
		float M1;
		float M2;
		float M3;
		float M4;
};

struct PhysicalProperties {
	PhysicalProperties(const nlohmann::json &json_str) : 
	roll(json_str, "roll"), pitch(json_str, "pitch"), yaw(json_str, "yaw"), moments(json_str) {
		mass = json_str["mass"];
		motor_radius = json_str["motor_radius"];
	}
	double mass;			// [kg]
	double motor_radius;	// [m]
	Gains roll;
	Gains pitch;
	Gains yaw;
	MomentsInertia moments;
	TorqueContribution taux;
	TorqueContribution tauy;
	TorqueContribution tauz;
};

struct Thrust_4M {
	double T1;
	double T2;
	double T3;
	double T4;
};

}

#endif