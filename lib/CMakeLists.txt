cmake_minimum_required(VERSION 3.15)
project(hack_lifter_lib)

find_package(LLVM REQUIRED CONFIG)
#include_directories(${LLVM_INCLUDE_DIRS})
#add_definitions(${LLVM_DEFINITIONS})

#message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(SOURCES
        hack_lifter/src/BuilderHelper.cpp
        hack_lifter/src/ModuleTools.cpp
        hack_lifter/src/IREmitter.cpp
        hack_lifter/src/BlockCache.cpp
        hack_lifter/testing/create_void_function.cpp
        hack_lifter/src/BlockMap.cpp
        hack_lifter/src/parse_asm.cpp
        )

add_library(hack_lifter ${SOURCES})
target_link_libraries(hack_lifter PUBLIC hack_assembler LLVM)
target_include_directories(hack_lifter PUBLIC include ${LLVM_INCLUDE_DIRS})

add_library(hac_lifter_embedded hack_lifter/embed/embed.cpp)