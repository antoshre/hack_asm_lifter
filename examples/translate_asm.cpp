//
// Created by rob on 7/18/2020.
//

#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <llvm/Support/raw_ostream.h>
#include "hacklift/create_void_function.h"
#include "hacklift/ModuleTools.h"
#include "hackasm/AsmFile.h"
#include "hacklift/parse_asm.h"

using namespace llvm;

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: [exe] [ASM filename]" << std::endl;
        return -1;
    }

    /*
     * Open the assembly file for reading.
     */
    std::ifstream file(argv[1], std::ios::in);
    if (!file) {
        std::cerr << "Could not open file: " << argv[1] << std::endl;
        return -2;
    }
    /*
     * See github.com/antoshre/hack_asm_assembler for details.
     */
    hackasm::AST ast(file);

    auto ctx = std::make_unique<LLVMContext>();
    auto mod = std::make_unique<Module>("module", *ctx);

    /*
     * Parse the AST into an LLVM module named "module", and create a void(*)(int16_t*) function named "f".
     * This is where all the IR generation happens.
     */
    hacklift::parse_asm_file(*mod, ast);

    std::cout << "Unoptimized: (" << mod->getFunction("f")->getInstructionCount() << " insts)" << std::endl;
    hacklift::print_module(*mod);

    /*
     * Use llvm::verifyModule to check the structure of the generated IR.
     * This will throw an exception if a problem is found.
     */
    hacklift::verify_module(*mod);

    /*
     * Optional.  Runs the equivalent of "-O3" optimization on the generated IR.
     */
    hacklift::optimize_module(*mod);

    std::cout << "\nOptimized: (" << mod->getFunction("f")->getInstructionCount() << " insts)" << std::endl;
    hacklift::print_module(*mod);

    std::cout << "Labels:\n";
    for (const auto&[k, v] : ast.get_symbol_table().get_labels()) {
        std::cout << k << " : " << v << '\n';
    }

    /*
     * The generated IR assumes an int16_t* is passed in as a parameter,
     * this represents the "memory" for the transpiled program to work on.
     * Until keyboard and screen handling are in this is the only way to get data into and out of the system.
     * Soon (TM) it will also return the contents of the A and D registers.
     */
    std::array<int16_t, 32768> mem{0};
    mem[0] = 50;
    mem[1] = 85;

    /*
     * hacklift::run_void_func will print the first 16 elements of the memory array before and after the IR is run.
     * Eg:
     * Memory before run:
     * 0032 0055 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
     * Memory after run:
     * 0032 0055 0087 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
     *
     *  WARNING: The structure of the code to run the module require it to be moved so `mod` should be considered dead!
     */
    hacklift::run_void_func(std::move(mod), std::move(ctx), "f", mem);

    return 0;
}

