set(SOURCE "rasfly_config.cpp" "rasfly_esc.cpp" "rasfly_imu.cpp" "rasfly_imu_py.cpp" "rasfly_control.cpp")

find_package(PythonLibs REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

if(CMAKE_BUILD_TYPE MATCHES SHARED)
	add_library(rasfly_lib SHARED ${SOURCE})
else(CMAKE_BUILD_TYPE MATCHES SHARED)
	add_library(rasfly_lib STATIC ${SOURCE})
endif(CMAKE_BUILD_TYPE MATCHES SHARED)
target_link_libraries(rasfly_lib pigpio pthread ${PYTHON_LIBRARIES})
target_link_libraries (rasfly_lib Eigen3::Eigen)
target_include_directories(rasfly_lib PUBLIC ${PIGPIO_INCLUDE_DIR})
target_include_directories(rasfly_lib PUBLIC ${EIGEN3_INCLUDE_DIR})
target_include_directories(rasfly_lib PUBLIC ${PYTHON_INCLUDE_DIRS})



set(HEADERS "rasfly_config.h" "rasfly_esc.h" "rasfly_types.h")
set(SCRIPTS "${PROJECT_SOURCE_DIR}/scripts/install.sh")
set(CONFIG "${PROJECT_SOURCE_DIR}/rasfly.config")

set(CMAKE_INSTALL_PREFIX rasfly)

if(CMAKE_BUILD_TYPE MATCHES SHARED)
	install(TARGETS rasfly_lib LIBRARY DESTINATION lib)
	install(FILES ${HEADERS} DESTINATION include)
endif(CMAKE_BUILD_TYPE MATCHES SHARED)
install(FILES ${HEADERS} DESTINATION include)
install(FILES ${SCRIPTS} DESTINATION scripts)
install(FILES ${CONFIG} DESTINATION ./)

set(CPACK_SOURCE_GENERATOR "TGZ")

include(CPack)