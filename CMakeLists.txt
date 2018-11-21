cmake_minimum_required (VERSION 3.2)
project(rasfly)

set(CMAKE_CXX_STANDARD 14)

include_directories (src)
add_subdirectory(src)

add_executable(rasfly_app src/main.cpp)
target_link_libraries(rasfly_app rasfly_lib)
target_link_libraries(rasfly_app pigpio pthread)
