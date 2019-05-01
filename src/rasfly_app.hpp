#ifndef _RASFLY_H_
#define _RASFLY_H_

#include <memory>
#include "json.hpp"
#include "rasfly_plugins.hpp"
#include "rasfly_imu.hpp"
#include "rasfly_controller.hpp"

namespace rasfly {
class rasfly_app {
public:
	rasfly_app();
	void BindIMU();
	void run();
private:
	nlohmann::json config;
	Plugins plugins;
	std::unique_ptr<IMU> _imu;
	// std::unique_ptr<filter> _filter;
	std::unique_ptr<Controller> _controller;
	// std::unique_ptr<esc> _esc;
};

}

#endif