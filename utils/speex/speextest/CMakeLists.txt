cmake_minimum_required (VERSION 3.14)
#project (speex-module CXX)

include (FindPkgConfig)

set (CMAKE_CXX_STANDARD 11)

add_executable(speex-module 
	main.cpp
)

pkg_check_modules(DSP REQUIRED speexdsp)

include_directories (
	speex-module
	${DSP_INCLUDE_DIRS}
)

target_link_directories (
	speex-module PUBLIC
	${DSP_LIBRARY_DIRS}
)

target_link_libraries (
	speex-module
	${DSP_LINK_LIBRARIES}
)
