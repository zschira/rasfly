#ifndef _RASFLY_TYPES_H_
#define _RASFLY_TYPES_H_

struct Quaternion {
	double w;
	double x;
	double y;
	double z;
};

struct State {
	// Position
	double x;
	double y;
	double z;
	Quaternion quaternion;
};

#endif