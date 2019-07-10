#include "log.hpp"

//================================================================================
// Global log file used by entire program
//================================================================================
std::chrono::system_clock::time_point LogFile::start = std::chrono::system_clock::now();
std::ofstream LogFile::file;
