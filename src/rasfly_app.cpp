#include "rasfly_app.hpp"
#include <fstream>
#include <iostream>

rasfly::rasfly_app::rasfly_app() {
	_imu = std::make_unique<IMU>();
	_controller = std::make_unique<Controller>();
	std::ifstream config_file("rasfly.json");
	config_file >> config;
	BindIMU();
}

void rasfly::rasfly_app::BindIMU() {
	if(plugins.IsImplemented("imu")) {
		_imu->getState = [this]() -> State {
			return plugins.GetState();
		};
	}

	if(plugins.IsImplemented("controller")) {
		_controller->calcThrust = [this](State s) -> Thrust {
			return plugins.CalcThrust();
		};
	}
}

void rasfly::rasfly_app::run() {
	auto curr = _imu->getState();
	std::cout << curr.x << "\n";
	std::cout << curr.quaternion.z << "\n";
}