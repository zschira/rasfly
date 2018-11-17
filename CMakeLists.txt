cmake_minimum_required (VERSION 3.2)
project(RASFLY)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_BUILD_TYPE MATCHES Static)
	add_library(RASFLY STATIC src/rasfly.cpp)
else(CMAKE_BUILD_TYPE MATCHES Static)
	add_library(RASFLY SHARED src/rasfly.cpp)
endif(CMAKE_BUILD_TYPE MATCHES Static)
target_link_libraries(RASFLY pigpio pthread)

set(HEADERS "src/rasfly.h" "src/config.h")
add_executable(test src/test.cpp)
target_link_libraries(test RASFLY)

install(TARGETS RASFLY DESTINATION ${CMAKE_SYSROOT}/usr/lib)
install(FILES ${HEADERS} DESTINATION ${CMAKE_SYSROOT}/usr/include)
