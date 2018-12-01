#include "rasfly_types.h"
#include "rasfly_config.h"
#include "rasfly_esc.h"

#include <vector>
#include <iostream>

int main() {
	std::cout << "Starting Up\n";
	// Struct to hold all hardware configuration
	rasfly::hardware raspi;
	// Read Config file and set hardware settings
	rasfly::config configuration("/home/alarm/rasfly.config");
	configuration.readConfig(raspi);
	// Initialize speed controllers/motors
	rasfly::esc esc_vec[NUM_MOTORS];
	int counter = 0;
	for(int pin : raspi.esc_pins) {
		// Initialize speed controller and add to vector
		esc_vec[counter] = rasfly::esc(pin, raspi.esc_rate, raspi.esc_range);
		counter++;	
	}
	// Bind imu driver

	std::cout << "Powering Down\n";
	return 0;
}