#include "rasfly_types.h"
#include "rasfly_config.h"
#include "rasfly_esc.h"
#include "rasfly_imu.h"

#include <vector>
#include <iostream>

int main() {
	std::cout << "Starting Up\n";
	// Read Config file and set hardware settings
	rasfly::config rasfly_config("/home/alarm/rasfly.config");
	auto configuration = rasfly_config.readConfig();
	// Initialize speed controllers/motors
	rasfly::esc esc_vec[NUM_MOTORS];
	int counter = 0;
	for(int pin : configuration.esc_pins) {
		// Initialize speed controller and add to vector
		esc_vec[counter] = rasfly::esc(pin, configuration.esc_rate, configuration.esc_range);
		counter++;	
	}
	// Bind imu driver
	rasfly::imu rasfly_imu;
	int err = rasfly_imu.loadIMU(configuration);
	// Read state from imu
	rasfly::state cstate;
	if(!err) {
		for(int i=0; i<100; i++) {
			rasfly_imu.getState(cstate);
			printf("quaternion: %f,%f,%f,%f\n", cstate.orientation.w, cstate.orientation.x, cstate.orientation.y, cstate.orientation.z);
		}
	}
	std::cout << "Powering Down\n";
	return 0;
}