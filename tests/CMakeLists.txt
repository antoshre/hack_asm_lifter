cmake_minimum_required(VERSION 3.15)
project(hack_lifter_tests)

find_package(GTest)
if (NOT GTEST_FOUND)
    message(STATUS "GTest package not found, pulling from remote")
    #Make GTest targets available
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.10.0
    )
    #Disable GMock, it isn't used
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    #Populate gtest gtest_main targets of googletest project
    FetchContent_MakeAvailable(googletest)
endif ()

add_executable(add_asm_functional add_asm.cpp)
target_link_libraries(add_asm_functional hack::lifter gtest gtest_main)