cmake_minimum_required(VERSION 3.15)
project(hack_lifter)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(external/hackasm)

find_package(LLVM REQUIRED)

add_executable(hack_lifter main.cpp src/BuilderHelper.cpp src/BuilderHelper.h examples/simple_add_mod.cpp src/ModuleOptimizer.cpp src/ASTTransformer.cpp)
target_link_libraries(hack_lifter hack_assembler llvm)