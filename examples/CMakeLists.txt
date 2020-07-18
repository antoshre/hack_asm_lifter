cmake_minimum_required(VERSION 3.15)
project(examples)

add_executable(translate_Add translate_Add.cpp)
target_link_libraries(translate_Add hack_lifter)

add_executable(print_module print_module.cpp)
target_link_libraries(print_module hack_lifter)