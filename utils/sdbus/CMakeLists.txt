cmake_minimum_required (VERSION 3.1)
project (sdbus-test C
	CXX)

include (FindPkgConfig)

set (CMAKE_CXX_STANDARD 14)
find_package(Threads REQUIRED)

add_executable(sdbus-test 
	service_exam.c
)

add_library(
	fka-sdbus-proxy SHARED
	SdBusProxy.cc
)

pkg_check_modules(SYSD REQUIRED libsystemd)

include_directories (
	sdbus-test
	${SYSD_INCLUDE_DIRS}
)
include_directories (
	fka-sdbus-proxy
	${SYSD_INCLUDE_DIRS}
)

target_link_directories (
	sdbus-test PUBLIC
	${SYSD_LIBRARY_DIRS}
)
target_link_directories (
	fka-sdbus-proxy PUBLIC
	${SYSD_LIBRARY_DIRS}
)

target_link_libraries (
	sdbus-test
	${SYSD_LINK_LIBRARIES}
)
target_link_libraries (
	fka-sdbus-proxy
	${SYSD_LINK_LIBRARIES}
)
