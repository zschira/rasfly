#include "rasfly_motors.hpp"
#include "log.hpp"
#include <vector>

struct rasfly::Motors::DefaultMotors {
    DefaultMotors(unsigned _range, unsigned _rate, unsigned _min_pulse, unsigned _max_pulse);
    void SetThrust(Thrust &thrust);
    std::vector<unsigned> gpio_pins;
    unsigned min_throttle, max_throttle;
    unsigned throttle_diff;
    unsigned min_pulse, max_pulse;
    unsigned range;
    unsigned rate;
};

rasfly::Motors::DefaultMotors::DefaultMotors(unsigned _range, unsigned _rate, unsigned _min_pulse, unsigned _max_pulse) {
    range = _range;
    rate = _rate;
    min_pulse = _min_pulse;
    max_pulse = _max_pulse;

    min_throttle = min_pulse * 1000/rate * range;

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
}

void rasfly::Motors::DefaultMotors::SetThrust(Thrust &thrusts) {
    gpioPWM(gpio_pins[0], min_throttle + range*thrusts.T1);
    gpioPWM(gpio_pins[1], min_throttle + range*thrusts.T2);
    gpioPWM(gpio_pins[2], min_throttle + range*thrusts.T3);
    gpioPWM(gpio_pins[3], min_throttle + range*thrusts.T4);
}

rasfly::Motors::Motors() {
    pImpl = std::make_unique<DefaultMotors>();
    setThrust = [this](Thrust &thrusts) {
        return pImpl->SetThrust(thrusts);
    };
}