cmake_minimum_required(VERSION 3.9)
project (mptest
	C CXX)

include(FindPkgConfig)
find_package(OpenMP REQUIRED)

set (CMAKE_CXX_STANDARD 11)

add_executable(mptest
	main.cc
)

add_executable(atest
	test.cc
)

target_link_libraries(mptest
	PUBLIC
	OpenMP::OpenMP_CXX
)

target_link_libraries(atest
	PUBLIC
	OpenMP::OpenMP_CXX
)
