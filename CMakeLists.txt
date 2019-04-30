project(rasfly)
cmake_minimum_required (VERSION 3.2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)

add_executable(rasfly_app src/main.cpp)
target_link_libraries(rasfly_app rasfly_lib)
# target_link_libraries(rasfly_app pigpio pthread)
