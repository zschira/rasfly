#ifndef _RASFLY_PLUGINS_H_
#define _RASFLY_PLUGINS_H_

#include "rasfly_types.hpp"
#include <map>
#include <memory>

namespace rasfly {

class Plugins {
public:
	Plugins();
	~Plugins();	
	State GetState();
	Thrust CalcThrust();
	bool IsImplemented(const char *function);
private:
	void BindPlugins();
	struct PyObjs;
	std::unique_ptr<PyObjs> pobjs;
	std::map<const char *, bool> function_implemented;
	const char *driver_name, *class_name;
};

}

#endif