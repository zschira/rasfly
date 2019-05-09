#include "rasfly_app.hpp"
#include <fstream>
#include <iostream>

rasfly::rasfly_app::rasfly_app() {
	std::ifstream config_file("rasfly.json");
	config_file >> config;
	_imu = std::make_unique<IMU>();
	_filter = std::make_unique<Filter>();
	_controller = std::make_unique<Controller>(config);
	BindCallbacks();
}

void rasfly::rasfly_app::BindCallbacks() {
	if(plugins.IsImplemented("imu")) {
		_imu->getState = [this]() -> State {
			plugins.Execute("imu");
			return plugins.api_state;
		};
	}

	if(plugins.IsImplemented("filter")) {
		_filter->filterState = [this](State) -> State {
			plugins.Execute("filter");
			return plugins.api_state;
		};
	}

	if(plugins.IsImplemented("controller")) {
		_controller->calcThrust = [this](State&, State&, double) -> Thrust_4M {
			plugins.Execute("controller");
			return plugins.api_thrust;
		};
	}
}

void rasfly::rasfly_app::run() {
	State trime;
	auto curr = _imu->getState();
	auto filtered = _filter->filterState(curr);
	auto thrust = _controller->calcThrust(curr, trime, 0.5);
}