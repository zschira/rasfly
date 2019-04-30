#include "rasfly_app.hpp"
#include <fstream>
#include <iostream>

rasfly::rasfly_app::rasfly_app() {
	std::ifstream config_file("rasfly.json");
	config_file >> config;
}