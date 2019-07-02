#ifndef _LOG_H_
#define _LOG_H_

#include <fstream>
#include <iostream>
enum class Level {
	INFO,
	WARNING,
	ERROR,
};

//////////////////////////////////////////////////////////////////////////////////
/// @class LogFile
///
/// @brief Accepts and forwards streams to ofstream writing to a logfile 
//////////////////////////////////////////////////////////////////////////////////
class LogFile {
public:
	static void StartLogging() {
		file.open("rasfly_log");
	}

	template<class T>
	LogFile& operator<< (T &input) {
		file << input;
		return *this;
	}

private:
	static std::ofstream file;
};

//////////////////////////////////////////////////////////////////////////////////
/// @class Log
///
/// @brief Accepts and forwards streams to ofstream writing to a logfile 
//////////////////////////////////////////////////////////////////////////////////
template<Level level>
class Log {
public:
	Log() {
		if constexpr(level == Level::INFO) {
			log << "INFO: ";
		} else if constexpr(level == Level::WARNING) {
			log << "WARNING: ";
		} else if constexpr(level == Level::ERROR) {
			log << "ERROR: ";
		}
	}

	Log(const bool _write) {
		write = _write;
		if(write) {
			if constexpr(level == Level::INFO) {
				log << "INFO: ";	
			} else if constexpr(level == Level::WARNING) {
				log << "WARNING: ";
			} else if constexpr(level == Level::ERROR) {
				log << "ERROR: ";
			}
		}
	}

	~Log() {
		if(write)
			log << "\n";
		if constexpr(level == Level::ERROR) {
			exit(1);
		}
	}

	template<class T>
	inline Log<level>& operator<< (T &input) {
		if(write)
			log << input;
		return *this;
	}

private:
	LogFile log;
	bool write = true;
};

#endif