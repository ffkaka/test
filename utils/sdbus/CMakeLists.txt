cmake_minimum_required (VERSION 3.14)
project (sdbus-test C)

include (FindPkgConfig)

#set (CMAKE_CXX_STANDARD 11)
find_package(Threads REQUIRED)

add_executable(sdbus-test 
	service_exam.c
)

pkg_check_modules(SYSD REQUIRED libsystemd)

include_directories (
	sdbus-test
	${SYSD_INCLUDE_DIRS}
)

target_link_directories (
	sdbus-test PUBLIC
	${SYSD_LIBRARY_DIRS}
)

target_link_libraries (
	sdbus-test
	${SYSD_LINK_LIBRARIES}
)
