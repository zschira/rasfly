cmake_minimum_required (VERSION 3.2)
project(RASFLY)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_BUILD_TYPE MATCHES Static)
	add_library(RASFLY STATIC src/rasfly.cpp)
else(CMAKE_BUILD_TYPE MATCHES Static)
	add_library(RASFLY SHARED src/rasfly.cpp)
endif(CMAKE_BUILD_TYPE MATCHES Static)

install(TARGETS RASFLY DESTINATION lib)
install(FILES src/rasfly.h DESTINATION include)
