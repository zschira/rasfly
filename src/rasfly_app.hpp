#ifndef _RASFLY_H_
#define _RASFLY_H_

#include <memory>
#include "json.hpp"
#include "rasfly_plugins.hpp"

namespace rasfly {
class filter;
class controller;
class esc;

class rasfly_app {
public:
	rasfly_app();

private:
	nlohmann::json config;
	Plugins plugins;
	std::unique_ptr<imu> _imu;
	std::unique_ptr<filter> _filter;
	std::unique_ptr<controller> _controller;
	std::unique_ptr<esc> _esc;
};

}

#endif