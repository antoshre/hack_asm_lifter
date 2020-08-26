cmake_minimum_required(VERSION 3.15)
project(hack_lifter_tests)

#Disable GMock, it isn't used
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

find_package(GTest)
if (GTEST_FOUND)
    #Local GoogleTest package found
else ()
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

set(TEST_SOURCES
        functional.cpp)

add_executable(${PROJECT_NAME} ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME} hack_lifter gtest gtest_main)