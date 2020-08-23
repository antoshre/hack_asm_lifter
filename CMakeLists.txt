cmake_minimum_required(VERSION 3.15)
project(hack_lifter)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external)

add_subdirectory(lib) #target: hack_lifter

option(BUILD_EXAMPLES "Build examples" ON)
#Only enable examples if this is the root project
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

option(BUILD_TESTING "Build tests" ON)
#Only enable tests if this is the root project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif ()
