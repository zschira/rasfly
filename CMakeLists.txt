cmake_minimum_required (VERSION 3.2)
project(RASFLY)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_BUILD_TYPE MATCHES Static)
	add_library(RASFLY STATIC src/rasfly.cpp)
else(CMAKE_BUILD_TYPE MATCHES Static)
	add_library(RASFLY SHARED src/rasfly.cpp)
endif(CMAKE_BUILD_TYPE MATCHES Static)
target_link_libraries(RASFLY pigpio pthread)

set(HEADERS "src/rasfly.h" "src/rasfly_config.h" "src/rasfly_types.h")
set(SCRIPTS "scripts/install.sh")
set(CONFIG "rasfly.config")

set(CMAKE_INSTALL_PREFIX RASFLY)

install(TARGETS RASFLY LIBRARY DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)
install(FILES ${SCRIPTS} DESTINATION scripts)
install(FILES ${CONFIG} DESTINATION ./)
install(TARGETS RASFLY LIBRARY DESTINATION ${CMAKE_SYSROOT}/usr/local/lib)
install(FILES ${HEADERS} DESTINATION ${CMAKE_SYSROOT}/usr/local/include)

set(CPACK_SOURCE_GENERATOR "TGZ")

include(CPack)