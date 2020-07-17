cmake_minimum_required(VERSION 3.15)
project(hack_lifter)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external/hackasm)

#TODO: figure out how to properly link LLVM in so it works
#Why doesn't the LLVM target have the includes attached?

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

#TODO: clean up the targets and split into lib/example/tests
add_executable(hack_lifter main.cpp src/BuilderHelper.cpp examples/simple_add_mod.cpp src/ModuleOptimizer.cpp src/ASTTransformer.cpp)

target_link_libraries(hack_lifter hack_assembler LLVM)