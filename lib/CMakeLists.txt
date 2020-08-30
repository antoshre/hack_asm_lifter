cmake_minimum_required(VERSION 3.15)
project(hack_lifter_lib)

find_package(LLVM "10.0.0" REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")


set(SOURCES
        src/BuilderHelper.cpp
        src/ModuleTools.cpp
        src/IREmitter.cpp
        src/BlockCache.cpp
        src/testing/create_void_function.cpp
        src/BlockMap.cpp
        src/parse_asm.cpp
        )

add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC hack::assembler LLVM)
target_include_directories(${PROJECT_NAME} PUBLIC include ${LLVM_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${LLVM_DEFINITIONS})

add_library(hack::lifter ALIAS ${PROJECT_NAME})