cmake_minimum_required (VERSION 3.14)

include (FindPkgConfig)

set (CMAKE_CXX_STANDARD 11)

add_executable(speex-test 
	main.cpp
)

pkg_check_modules(DSP REQUIRED speexdsp)

include_directories (
	speex-test
	../ka-src
	${DSP_INCLUDE_DIRS}
)

target_link_directories (
	speex-test PUBLIC
	${DSP_LIBRARY_DIRS}
)

target_link_libraries (
	speex-test
	${DSP_LINK_LIBRARIES}
)
