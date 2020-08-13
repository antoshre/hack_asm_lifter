cmake_minimum_required(VERSION 3.15)
project(hack_lifter_external_deps)

#Make hackasm targets available
include(FetchContent)
FetchContent_Declare(
        hackasm
        GIT_REPOSITORY https://github.com/antoshre/hack_asm_assembler.git
        GIT_TAG e609ae3d977aa94de1f77d6d836a225f43b00fad
)
FetchContent_MakeAvailable(hackasm)