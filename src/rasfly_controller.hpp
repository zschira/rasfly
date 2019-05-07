#ifndef _RASFLY_CONTROLLER_H_
#define _RASFLY_CONTROLLER_H_

#include <functional>
#include <memory>
#include "json.hpp"
#include "rasfly_types.hpp"

namespace rasfly {

class Controller {
public:
	Controller(nlohmann::json config);
	~Controller();
	std::function<Thrust_4M(State&, State&, double)> calcThrust;
private:
	struct DefaultController;
	std::unique_ptr<DefaultController> pimpl;
	PhysicalProperties properties;
};
}

#endif