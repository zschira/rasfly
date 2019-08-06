#include "rasfly_app.hpp"
#include "json.hpp"
#include "log.hpp"
#include "rasfly_util.hpp"
#include <bitset>

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
    _motors = ESCInterface::factory(config);
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
        Log<Level::INFO, true>() << "Using IMU plugin";

        _imu->getState = [this]() -> State {
            return plugins.Execute<State>("imu");
        };
    }

    if(plugins.IsImplemented("filter")) {
        Log<Level::INFO>() << "Using filter plugin";

        _filter->filterState = [this](State s) -> State {
            return plugins.Execute<State>("filter", &s);
        };
    }

    if(plugins.IsImplemented("inputs")) {
        Log<Level::INFO>() << "Using inputs plugin";

        _inputs->getPilotInput = [this]() -> State {
            return plugins.Execute<State>("inputs");
        };
    }

    if(plugins.IsImplemented("controller")) {
        Log<Level::INFO>() << "Using controller plugin";

        _controller->calcThrust = [this](State&, State&) -> Thrust {
            return plugins.Execute<Thrust>("controller");
        };
    }

    auto valid = CreateBitSet<3>(_imu->getState, _inputs->getPilotInput, _controller->calcThrust);
    if(!valid.all()) {
        Log<Level::ERROR>() << "Invalid Configuration: ";
    }
}

//////////////////////////////////////////////////////////////////////////////////
/// @brief Main loop simply calls each callback function
//////////////////////////////////////////////////////////////////////////////////
void rasfly::rasfly_app::run() {
    auto measured_state = _imu->getState();
    auto filtered_state = _filter->filterState(measured_state);
    auto pilot_input = _inputs->getPilotInput();
    auto thrust = _controller->calcThrust(filtered_state, pilot_input);
    _motors->setThrust(thrust);
}