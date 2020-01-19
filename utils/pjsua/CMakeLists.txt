cmake_minimum_required(VERSION 3.4)
project (sip-test
C CXX)

#set (CMAKE_CXX_STANDARD 14)

include(FindPkgConfig)
pkg_check_modules(PJSUA REQUIRED libpjproject)

add_executable(sip-test
	samples.cc
)

message(STATUS "Include Path = ${PJSUA_VERSION}")

include_directories (
	sip-test PUBLIC
	${PJSUA_INCLUDE_DIRS}
)

target_link_libraries (
	sip-test PUBLIC
	${PJSUA_LIBRARIES}
)
