cmake_minimum_required (VERSION 3.14)
#project (ka-src CXX)

include (FindPkgConfig)

set (CMAKE_CXX_STANDARD 11)

add_library(libka-src SHARED
	SpeexSrc.cpp
)

pkg_check_modules(DSP REQUIRED speexdsp)

include_directories (
	libka-src
	${DSP_INCLUDE_DIRS}
)

target_link_directories (
	libka-src PUBLIC
	${DSP_LIBRARY_DIRS}
)

target_link_libraries (
	libka-src
	${DSP_LINK_LIBRARIES}
)
