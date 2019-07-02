#include "rasfly_app.hpp"
#include "json.hpp"
#include "log.hpp"
#include <fstream>
#include <iostream>

//////////////////////////////////////////////////////////////////////////////////
/// @brief Default constructor
//////////////////////////////////////////////////////////////////////////////////
rasfly::rasfly_app::rasfly_app() {
	LogFile::StartLogging();
	nlohmann::json config;
	std::ifstream config_file("rasfly.json");
	config_file >> config;
	_imu = std::make_unique<IMU>();
	_filter = std::make_unique<Filter>();
	_controller = std::make_unique<Controller>(config);
	_motors = std::make_unique<Motors>();
	_inputs = std::make_unique<Inputs>();
	BindCallbacks();
}

//////////////////////////////////////////////////////////////////////////////////
/// @brief Check if there's a python implementation of each callback function. If
///        a python implementation exists it will ALWAYS be used. If such an
///        implemenation does not exist it will fall back on a c++ implementation.
///        If there is also no c++ implementation then an error will be logged and
///        the programe will stop 
//////////////////////////////////////////////////////////////////////////////////
void rasfly::rasfly_app::BindCallbacks() {
	if(plugins.IsImplemented("imu")) {
		_imu->getState = [this]() -> State {
			return plugins.Execute<State>("imu");
		};
	}

	if(plugins.IsImplemented("filter")) {
		_filter->filterState = [this](State s) -> State {
			return plugins.Execute<State>("filter", &s);
		};
	}

	if(plugins.IsImplemented("inputs")) {
		_inputs->getPilotInput = [this]() -> State {
			return plugins.Execute<State>("inputs");
		};
	}

	if(plugins.IsImplemented("controller")) {
		_controller->calcThrust = [this](State&, State&) -> Thrust_4M {
			return plugins.Execute<Thrust_4M>("controller");
		};
	}

	bool valid = _imu->getState && _inputs->getPilotInput && _controller->calcThrust && _motors->setThrust;
	if(!valid) {
		Log<Level::ERROR>() << "Invalid Configuration";
	}
}

//////////////////////////////////////////////////////////////////////////////////
/// @brief Main loop simply calls each callback function
//////////////////////////////////////////////////////////////////////////////////
void rasfly::rasfly_app::run() {
	auto curr = _imu->getState();
	auto filtered = _filter->filterState(curr);
	auto pilot_input = _inputs->getPilotInput();
	auto thrust = _controller->calcThrust(filtered, pilot_input);
	_motors->setThrust(thrust);
}