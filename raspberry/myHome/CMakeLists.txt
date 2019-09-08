cmake_minimum_required (VERSION 3.1)
project (HomeControl C
	CXX)

include (FindPkgConfig)

set (CMAKE_CXX_STANDARD 14)
find_package(Threads REQUIRED)

add_executable (
	home-ctrl
	scd/scd30.c
	scd/scd_git_version.c
	scd/sensirion_common.c
	#scd/sensirion_hw_i2c_implementation.c
	scd/sensirion_sw_i2c.c
	scd/sensirion_sw_i2c_implementation.c
	check_environ.cc
)

pkg_check_modules(PULSE REQUIRED libpulse)
pkg_check_modules(PULSE_SIMPLE REQUIRED libpulse-simple)

include_directories (
	home-ctrl
	scd/
	${PULSE_SIMPLE_INCLUDE_DIRS}
	${PULSE_INCLUDE_DIRS}
)

target_link_directories (
	home-ctrl PUBLIC
	${PULSE_SIMPLE_LIBRARY_DIRS}
	${PULSE_LIBRARY_DIRS}
)

target_link_libraries (
	home-ctrl
	${PULSE_SIMPLE_LINK_LIBRARIES}
	${PULSE_LINK_LIBRARIES}
)
