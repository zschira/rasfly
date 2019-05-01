#include "rasfly_app.hpp"
#include <fstream>
#include <iostream>

rasfly::rasfly_app::rasfly_app() {
	_imu = std::make_unique<IMU>();
	std::ifstream config_file("rasfly.json");
	config_file >> config;
	BindIMU();
}

void rasfly::rasfly_app::BindIMU() {
	_imu->getState = [this]() -> State {
		return plugins.GetState();
	};
}

void rasfly::rasfly_app::run() {
	auto curr = _imu->getState();
	std::cout << curr.x << "\n";
}