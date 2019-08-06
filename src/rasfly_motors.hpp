#ifndef _RASFLY_MOTORS_H_
#define _RASFLY_MOTORS_H_

#include <functional>
#include <vector>
#include <memory>
#include <map>
#include "json.hpp"
#include "rasfly_types.hpp"

namespace rasfly {

enum class ESC{
    PWM,
    ONE_SHOT_125,
    ONE_SHOT_42,
    MULTISHOT,
    DSHOT,
};

//////////////////////////////////////////////////////////////////////////////////
/// @class ESCInterface
//////////////////////////////////////////////////////////////////////////////////
class ESCInterface {
public:
    static std::unique_ptr<ESCInterface> factory(nlohmann::json config);
    virtual void setThrust(Thrust &thrust) = 0;
protected:
    std::vector<unsigned> gpio_pins;
};

class AnalogSignal : public ESCInterface {
    AnalogSignal(unsigned range, unsigned rate, unsigned min_pulse, unsigned max_pulse);
    void setThrust(Thrust &thrust) override;
private:
    unsigned range;
    unsigned rate;
    unsigned min_pulse;
    unsigned max_pulse;
    unsigned min_input;
    unsigned input_diff;
};

}

#endif