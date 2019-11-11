cmake_minimum_required (VERSION 3.14)
#project (ka-src CXX)

include (FindPkgConfig)

set (CMAKE_CXX_STANDARD 11)

add_library(ka-src SHARED
	SpeexSrc.cc
)

pkg_check_modules(DSP REQUIRED speexdsp)

include_directories (
	ka-src
	${DSP_INCLUDE_DIRS}
)

target_link_directories (
	ka-src PUBLIC
	${DSP_LIBRARY_DIRS}
)

target_link_libraries (
	ka-src
	${DSP_LINK_LIBRARIES}
)

install (TARGETS ka-src LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
