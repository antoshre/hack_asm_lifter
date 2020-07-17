cmake_minimum_required(VERSION 3.15)
project(hack_lifter)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external/hackasm)

add_subdirectory(lib) #target: hack_lifter

option(BUILD_EXAMPLES "Build examples" ON)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
