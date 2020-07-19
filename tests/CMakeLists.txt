cmake_minimum_required(VERSION 3.15)
project(hack_lifter)

set(TEST_SOURCES
        instruction_count.cpp
        )

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME}_tests hack_lifter gtest gtest_main)