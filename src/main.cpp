#include "rasfly_types.h"
#include "rasfly_config.h"
#include "rasfly_esc.h"
#include "rasfly_imu.h"
#include "rasfly_control.h"

#include <vector>
#include <iostream>

int main() {
	std::cout << "Starting Up\n";
	// Read Config file and set hardware settings
	auto configuration = rasfly::readConfig("/home/alarm/rasfly/rasfly.config");
	// Initialize speed controllers/motors
	rasfly::esc esc_vec[NUM_MOTORS];
	int counter = 0;
	for(int pin : configuration.esc_pins) {
		// Initialize speed controller and add to vector
		esc_vec[counter] = rasfly::esc(pin, configuration.esc_rate, configuration.esc_range);
		counter++;	
	}
	// Bind imu driver
	auto imu = rasfly::imu::imu_factory(configuration.imu_driver, configuration.imu_path.c_str());
	// Read state from imu
	rasfly::state cstate, trim;
	trim.euler(1) = 0.087;
	trim.thrust = 0.5;
	rasfly::controller pid(configuration);
	for(int i=0; i<100; i++) {
		imu->getState(cstate);
		printf("%f,%f,%f\n", cstate.euler(0), cstate.euler(1), cstate.euler(2));
		auto thrust = pid.calculateThrust(cstate, trim);
		printf("Thrusts: %f,%f,%f,%f\n", thrust.T1, thrust.T2, thrust.T3, thrust.T4);
	}
	std::cout << "Powering Down\n";
	return 0;
}