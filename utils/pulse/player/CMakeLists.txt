cmake_minimum_required (VERSION 3.1)
project (pulse-player C
	CXX)

include (FindPkgConfig)

set (CMAKE_CXX_STANDARD 14)
find_package(Threads REQUIRED)

add_library(
	fka-pplayer SHARED
	PulsePlayer.cc
)

pkg_check_modules(PULSE REQUIRED libpulse)
pkg_check_modules(PULSE_SIMPLE REQUIRED libpulse-simple)

include_directories (
	fka-pplayer
	${PULSE_SIMPLE_INCLUDE_DIRS}
	${PULSE_INCLUDE_DIRS}
)

target_link_directories (
	fka-pplayer PUBLIC
	${PULSE_SIMPLE_LIBRARY_DIRS}
	${PULSE_LIBRARY_DIRS}
)

target_link_libraries (
	fka-pplayer
	${PULSE_SIMPLE_LINK_LIBRARIES}
	${PULSE_LINK_LIBRARIES}
)
