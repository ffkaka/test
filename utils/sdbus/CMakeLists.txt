cmake_minimum_required (VERSION 3.1)
project (sdbus-test CXX)

include (FindPkgConfig)

find_package(Threads REQUIRED)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_COMPILER /usr/bin/clang++-6.0)

add_executable(server
	service_exam.cc
	SdBusProxy.cc
)

add_executable(client
	client.cc
)

add_library(ka-SdBus SHARED
	SdBusProxy.cc
)

pkg_check_modules(SYSD REQUIRED libsystemd)

include_directories (
	server
	${SYSD_INCLUDE_DIRS}
)

target_link_directories (
	server PUBLIC
	${SYSD_LIBRARY_DIRS}
)

target_link_libraries (
	server
	Threads::Threads
	${SYSD_LINK_LIBRARIES}
)


include_directories (
	client
	${SYSD_INCLUDE_DIRS}
)
target_link_directories (
	client PUBLIC
	${SYSD_LIBRARY_DIRS}
)
target_link_libraries (
	client
	Threads::Threads
	${SYSD_LINK_LIBRARIES}
)
