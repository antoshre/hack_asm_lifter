//
// Created by rob on 7/16/2020.
//


#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/TargetSelect.h>
#include <iostream>
#include <iomanip>
#include "ModuleTools.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IRPrintingPasses.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"

#include "llvm/IR/AssemblyAnnotationWriter.h"

using namespace llvm;

namespace hacklift {

    //TODO: Can all this be static?
    void do_passes(llvm::Module* module, bool optimize=false, bool print_module=false) {


        //Optimization setup based on https://github.com/bollu/simplexhc-cpp/blob/master/src/main.cpp

        PassBuilder PB;

        ModulePassManager MPM;
        FunctionPassManager FPM;
        CGSCCPassManager CGSCCPM;

        if (optimize) {
            PassBuilder::OptimizationLevel optimisationLevel = PassBuilder::OptimizationLevel::O3;
            MPM = PB.buildModuleSimplificationPipeline(optimisationLevel, PassBuilder::ThinLTOPhase::None);
            FPM = PB.buildFunctionSimplificationPipeline(optimisationLevel, PassBuilder::ThinLTOPhase::None);
        }
        LoopAnalysisManager LAM;
        FunctionAnalysisManager FAM;
        CGSCCAnalysisManager CGAM;
        ModuleAnalysisManager MAM;

        // Register the AA manager first so that our version is the one used.
        if (optimize) {
            FAM.registerPass([&] { return PB.buildDefaultAAPipeline(); });
        }

        PB.registerModuleAnalyses(MAM);
        PB.registerCGSCCAnalyses(CGAM);
        PB.registerFunctionAnalyses(FAM);
        PB.registerLoopAnalyses(LAM);
        PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

        // Fix the IR first, then run optimisations.
        MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
        MPM.addPass(createModuleToPostOrderCGSCCPassAdaptor(std::move(CGSCCPM)));

        //Print module IR
        if (print_module) {
            MPM.addPass(PrintModulePass(llvm::outs()));
        }
        // We need to run the pipeline once for correctness. Anything after that
        // is optimisation.
        MPM.run(*module, MAM);
    }

    void optimize_module(llvm::Module* module) {
        //Module must be valid for optimization to make sense
        verifyModule(*module, &llvm::outs());
        do_passes(module, true, false);
    }

    void print_module(llvm::Module* module) {
        //do_passes(module, false, true);
        AssemblyAnnotationWriter aaw;
        module->print(llvm::outs(), &aaw);
    }

    int run(std::unique_ptr<Module>& module, std::unique_ptr<LLVMContext>& ctx, const std::string& func_name) {

    //InitLLVM X{1,""};
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();

    auto tsm = orc::ThreadSafeModule(std::move(module), std::move(ctx));

    auto JIT = orc::LLJITBuilder().create();
    if (!JIT) {
        llvm::outs() << JIT.takeError();
        return -1;
    }

    if (auto err = JIT->get()->addIRModule(std::move(tsm))) {
        llvm::outs() << err;
        return -2;
    }

    auto fsym = JIT->get()->lookup("foo");
    if (!fsym) {
        llvm::outs() << fsym.takeError();
        return -3;
    }
    auto f = (void (*)(int16_t*)) fsym.get().getAddress();
    int16_t mem[16];
    std::memset(mem, 0, sizeof(mem));

    mem[0] = 42;
    mem[1] = -2;
    std::cout << "Memory before run:\n";
    for(int i=0; i < 16; i++) {
        std::cout << std::hex << std::setw(4) << std::setfill('0') <<  mem[i] << ' ';
    }
    std::cout << '\n';
    f(mem);

    std::cout << "Memory after run:\n";
        for(int i=0; i < 16; i++) {
            std::cout << std::hex << std::setw(4) << std::setfill('0') <<  mem[i] << ' ';
        }
        std::cout << '\n';

        /*
    int16_t Mem[16];
    std::memset(Mem, 0, sizeof(Mem));
    Mem[0] = 4;
    Mem[1] = 5;
    std::cout << "Starting Mem:\n";
    for(int i=0; i < 16; i++) {
        std::cout << std::hex << std::setw(2) << Mem[i] << ' ';
    }
    std::cout << "\nCalling foo(Mem)\n";
    f(Mem);
    */
        return 0;
    }

    void verify_module(llvm::Module *module) {
        verifyModule(*module, &llvm::outs());
    }
}
