#include "rasfly_motors.hpp"
#include "log.hpp"

std::unique_ptr<rasfly::ESCInterface> rasfly::ESCInterface::factory(nlohmann::json config) {
        if(config["ESC_PROTOCOL"] == "PWM") {
            return std::make_unique<AnalogSignal>(4000, 50, 1, 2);
        } else if (config["ESC_PROTOCOL"] == "ONE_SHOT_125") {
            Log<Level::ERROR>() << "ONE_SHOT_125 not implemented";
        } else if (config["ESC_PROTOCOL"] == "ONE_SHOT_42") {
            Log<Level::ERROR>() << "ONE_SHOT_42 not implemented";
        } else if (config["ESC_PROTOCOL"] == "MULTISHOT") {
            Log<Level::ERROR>() << "MULTISHOT not implemented";
        } else if (config["ESC_PROTOCOL"] == "DSHOT") {
            Log<Level::ERROR>() << "DSHOT not implemented";
        } else {
            Log<Level::ERROR>() << "INVALID ESC_PROTOCOL: " << config["ESC_PROTOCOL"];
        }
        return nullptr;
}

rasfly::AnalogSignal::AnalogSignal(unsigned range, unsigned rate, unsigned min_pulse, unsigned max_pulse) {
    // Initialize pigpio library
    if(gpioInitialize() < 0) {
        Log<Level::ERROR>() << "Can't initialize pigpio";
    }

    for(auto pin : gpio_pins) {
        // Set pwm rate
        if(rate != gpioSetPWMfrequency(pin, rate)) {
            Log<Level::ERROR>() << "Can't set PWM frequency on pin: " << pin;
        }

        // Set pwm range
        if(range != gpioSetPWMrange(pin, range)) {
            Log<Level::ERROR>() << "Can't set PWM frequency on pin: " << pin;
        }
    }

    min_input = min_pulse * 1000/rate * range;
    auto max_input = max_pulse * 1000/rate * range;
    input_diff = max_input - min_input;
}

void rasfly::AnalogSignal::setThrust(Thrust &thrust) {
    gpioPWM(gpio_pins[0], min_input + thrust.T1*input_diff);
    gpioPWM(gpio_pins[1], min_input + thrust.T2*input_diff);
    gpioPWM(gpio_pins[2], min_input + thrust.T3*input_diff);
    gpioPWM(gpio_pins[3], min_input + thrust.T4*input_diff);
}