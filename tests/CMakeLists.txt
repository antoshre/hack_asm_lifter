cmake_minimum_required(VERSION 3.15)
project(hack_lifter_tests)

set(TEST_SOURCES
        functional.cpp)

add_executable(${PROJECT_NAME} ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME} hack_lifter gtest gtest_main)