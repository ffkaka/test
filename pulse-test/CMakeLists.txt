cmake_minimum_required (VERSION 3.14)
project (pulse-test CXX)

include (FindPkgConfig)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set (CMAKE_CXX_STANDARD 11)
find_package(Threads REQUIRED)

add_executable(pulse-test 
	pulse-input-handler.cpp
	record.cpp
	)

pkg_check_modules(PULSE REQUIRED libpulse)
pkg_check_modules(SIMPLE REQUIRED libpulse-simple)

include_directories (
	${PULSE_INCLUDE_DIRS}
	${SIMPLE_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/../utils/ring-buff/
)

target_link_directories (
	pulse-test PUBLIC
	${PULSE_LIBRARY_DIRS}
	${SIMPLE_LIBRARY_DIRS}
)

target_link_libraries (
	pulse-test
	${PULSE_LINK_LIBRARIES}
	${SIMPLE_LINK_LIBRARIES}
	Threads::Threads
)
