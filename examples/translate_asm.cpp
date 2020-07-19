//
// Created by rob on 7/18/2020.
//

#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <llvm/Support/raw_ostream.h>
#include "../lib/hack_lifter/testing/create_void_function.h"
#include "../src/ModuleTools.h"
#include "hackasm/AsmFile.h"
#include "../lib/hack_lifter/testing/parse_asm.h"

using namespace llvm;

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: [exe] [ASM filename]" << std::endl;
        return -1;
    }
    auto ctx = std::make_unique<LLVMContext>();
    auto mod = std::make_unique<Module>("module", *ctx);

    std::ifstream file(argv[1], std::ios::in);
    if (!file) {
        std::cerr << "Could not open file: " << argv[1] << std::endl;
        return -2;
    }
    hackasm::AsmFile asmfile(file);
    hackasm::AST ast(asmfile);
    hacklift::parse_asm_file(*mod, ast);

    std::cout << "Unoptimized: (" << mod->getFunction("f")->getInstructionCount() << " insts)" << std::endl;
    hacklift::print_module(*mod);

    hacklift::verify_module(*mod);

    hacklift::optimize_module(*mod);

    std::cout << "\nOptimized: (" << mod->getFunction("f")->getInstructionCount() << " insts)" << std::endl;
    hacklift::print_module(*mod);

    //Setup memory for transpiled code to act on
    std::array<int16_t, 32768> mem{0};
    mem[0] = 50;
    mem[1] = 85;

    //::run will print a before/after and return value
    hacklift::run_void_func(std::move(mod), std::move(ctx), "f", mem);

    return 0;
}

