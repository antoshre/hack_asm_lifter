cmake_minimum_required(VERSION 3.15)
project(hack_lifter_external_deps)

#Make hackasm targets available
include(FetchContent)
FetchContent_Declare(
        hackasm
        GIT_REPOSITORY https://github.com/antoshre/hack_asm_assembler.git
)
FetchContent_MakeAvailable(hackasm) #Target: hack::assembler