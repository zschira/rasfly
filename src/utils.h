#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <algorithm>

static inline void char_trim(std::string &s, const char delim) {
    	s.erase(std::find_if(s.begin(), s.end(), [&](int ch) {
        	return (ch == delim);
    	}), s.end());
}

static inline void erase_char(std::string &s, const char delim) {
    	s.erase(std::remove_if(s.begin(), s.end(), [&](int ch) {
        	return (ch == delim);
    	}), s.end());
}

static inline void erase_space( std::string &s) {
	s.erase(std::remove_if(s.begin(), s.end(), [](int ch) {
		return std::isspace(ch);
	}),s.end());
}

#endif