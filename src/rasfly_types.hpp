#ifndef _RASFLY_TYPES_H_
#define _RASFLY_TYPES_H_

#include "json.hpp"

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

#endif