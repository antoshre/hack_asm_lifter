cmake_minimum_required(VERSION 3.15)
project(hack_lifter_lib)

find_package(LLVM REQUIRED CONFIG)
#include_directories(${LLVM_INCLUDE_DIRS})
#add_definitions(${LLVM_DEFINITIONS})

#message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(SOURCES hack_lifter/src/ASTTransformer.cpp
        hack_lifter/src/BuilderHelper.cpp
        hack_lifter/src/ModuleTools.cpp
        )

add_library(hack_lifter ${SOURCES})
target_link_libraries(hack_lifter PUBLIC hack_assembler LLVM)
target_include_directories(hack_lifter PUBLIC hack_lifter/src ${LLVM_INCLUDE_DIRS})