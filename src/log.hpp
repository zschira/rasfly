#ifndef _LOG_H_
#define _LOG_H_

#include <fstream>
#include <iostream>
#include <chrono>
#include <map>

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
    LogFile& operator<< (T &&input) {
        file << input;
        return *this;
    }

    void PrintTimeStamp() {
        seconds elapsed_time = std::chrono::duration_cast<seconds>(std::chrono::system_clock::now() - start);
        file << "(" << elapsed_time.count() << ") ";
    }

    static std::chrono::system_clock::time_point start;
private:
    using seconds = std::chrono::duration<double>;
    static std::ofstream file;
};

//////////////////////////////////////////////////////////////////////////////////
/// @class Log
///
/// @brief Accepts and forwards streams to ofstream writing to a logfile 
//////////////////////////////////////////////////////////////////////////////////
template<Level level, bool printTime = false>
class Log {
public:
    Log() {
        if constexpr(printTime) {
            log.PrintTimeStamp();
        }
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
    inline Log<level, printTime>& operator<< (T &&input) {
        if(write)
            log << input;
        return *this;
    }

private:
    LogFile log;
    bool write = true;
};

class Error {
public:
    static void AddErrors(std::initializer_list<std::pair<unsigned long, char *>> list) {
        
    }
private:
    static std::map<unsigned long, char *> error_map;
};

#endif