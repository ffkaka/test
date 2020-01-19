cmake_minimum_required (VERSION 3.14)
project (libka-src CXX)

set (CMAKE_CXX_STANDARD 12)
set (CMAKE_CXX_COMPILIER /usr/bin/clang++-6.0)


add_subdirectory(ka-src)
add_subdirectory(speextest)


