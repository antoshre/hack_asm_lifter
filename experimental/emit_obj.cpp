//
// Created by rob on 7/18/2020.
//

#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include "hacklift/create_void_function.h"
#include "hacklift/ModuleTools.h"
#include "hackasm/AsmFile.h"
#include "hacklift/parse_asm.h"

#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetParser.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetMachine.h"

#include "llvm/IR/LegacyPassManager.h"

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

    //std::cout << "Unoptimized: (" << mod->getFunction("f")->getInstructionCount() << " insts)" << std::endl;
    //hacklift::print_module(*mod);

    /*
     * Use llvm::verifyModule to check the structure of the generated IR.
     * This will throw an exception if a problem is found.
     */
    hacklift::verify_module(*mod);

    /*
     * Optional.  Runs the equivalent of "-O3" optimization on the generated IR.
     */
    hacklift::optimize_module(*mod);

    //std::cout << "\nOptimized: (" << mod->getFunction("f")->getInstructionCount() << " insts)" << std::endl;
    //hacklift::print_module(*mod);

    auto target_triple = llvm::sys::getDefaultTargetTriple();
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();
    std::string Error;
    auto Target = TargetRegistry::lookupTarget(target_triple, Error);

// Print an error and exit if we couldn't find the requested target.
// This generally occurs if we've forgotten to initialise the
// TargetRegistry or we have a bogus target triple.
    if (!Target) {
        errs() << Error;
        return 1;
    }

    auto CPU = "generic";
    auto Features = "";

    TargetOptions opt;
    auto RM = Optional<Reloc::Model>();
    auto TargetMachine = Target->createTargetMachine(target_triple, CPU, Features, opt, RM);


    mod->setDataLayout(TargetMachine->createDataLayout());
    mod->setTargetTriple(target_triple);

    auto Filename = "output.o";
    std::error_code EC;
    raw_fd_ostream dest(Filename, EC, sys::fs::OpenFlags::OF_None);

    if (EC) {
        errs() << "Could not open file: " << EC.message();
        return 1;
    }

    legacy::PassManager pass;
    auto FileType = CGFT_ObjectFile;

    if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        errs() << "TargetMachine can't emit a file of this type";
        return 1;
    }

    pass.run(*mod);
    dest.flush();

    return 0;
}

