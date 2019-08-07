cmake_minimum_required (VERSION 3.1)
project (pulse-control C
	CXX)

include (FindPkgConfig)

set (CMAKE_CXX_STANDARD 14)
find_package(Threads REQUIRED)

add_library(
	fka-pcontrol SHARED
	PulseControls.cc
)

pkg_check_modules(PULSE REQUIRED libpulse)
pkg_check_modules(PULSE_SIMPLE REQUIRED libpulse-simple)

include_directories (
	fka-pcontrol
	${PULSE_SIMPLE_INCLUDE_DIRS}
	${PULSE_INCLUDE_DIRS}
)

target_link_directories (
	fka-pcontrol PUBLIC
	${PULSE_SIMPLE_LIBRARY_DIRS}
	${PULSE_LIBRARY_DIRS}
)

target_link_libraries (
	fka-pcontrol
	${PULSE_SIMPLE_LINK_LIBRARIES}
	${PULSE_LINK_LIBRARIES}
)
