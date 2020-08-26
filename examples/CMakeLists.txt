cmake_minimum_required(VERSION 3.15)
project(examples)

add_executable(print_module print_module.cpp)
target_link_libraries(print_module hack_lifter)

add_executable(translate_asm translate_asm.cpp)
target_link_libraries(translate_asm hack_lifter)

add_executable(emit_obj emit_obj.cpp)
target_link_libraries(emit_obj hack_lifter)