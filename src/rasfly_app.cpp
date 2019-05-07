#include "rasfly_app.hpp"
#include <fstream>
#include <iostream>

rasfly::rasfly_app::rasfly_app() {
	_imu = std::make_unique<IMU>();
	std::ifstream config_file("rasfly.json");
	config_file >> config;
	_controller = std::make_unique<Controller>(config);
	BindIMU();
}

void rasfly::rasfly_app::BindIMU() {
	if(plugins.IsImplemented("imu")) {
		_imu->getState = [this]() -> State {
			plugins.Execute("imu");
			return plugins.api_state;
		};
	}
}

void rasfly::rasfly_app::run() {
	State trime;
	auto curr = _imu->getState();
	auto thrust = _controller->calcThrust(curr, trime, 0.5);
}