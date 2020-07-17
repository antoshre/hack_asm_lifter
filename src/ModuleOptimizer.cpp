//
// Created by rob on 7/16/2020.
//


#include "ModuleOptimizer.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IRPrintingPasses.h"
#include "llvm/Passes/PassBuilder.h"

using namespace llvm;

namespace hacklift {
    void optimize(llvm::Module* module) {
        verifyModule(*module, &llvm::outs());

        //Optimization setup based on https://github.com/bollu/simplexhc-cpp/blob/master/src/main.cpp

        PassBuilder PB;

        ModulePassManager MPM;
        FunctionPassManager FPM;
        CGSCCPassManager CGSCCPM;

        PassBuilder::OptimizationLevel optimisationLevel = PassBuilder::OptimizationLevel::O3;
        MPM = PB.buildModuleSimplificationPipeline(optimisationLevel, PassBuilder::ThinLTOPhase::None);
        FPM = PB.buildFunctionSimplificationPipeline(optimisationLevel, PassBuilder::ThinLTOPhase::None);

        LoopAnalysisManager LAM;
        FunctionAnalysisManager FAM;
        CGSCCAnalysisManager CGAM;
        ModuleAnalysisManager MAM;

        // Register the AA manager first so that our version is the one used.
        FAM.registerPass([&] { return PB.buildDefaultAAPipeline(); });

        PB.registerModuleAnalyses(MAM);
        PB.registerCGSCCAnalyses(CGAM);
        PB.registerFunctionAnalyses(FAM);
        PB.registerLoopAnalyses(LAM);
        PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

        // Fix the IR first, then run optimisations.
        MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
        MPM.addPass(createModuleToPostOrderCGSCCPassAdaptor(std::move(CGSCCPM)));

        //Print module IR
        //MPM.addPass(PrintModulePass(llvm::outs()));

        // We need to run the pipeline once for correctness. Anything after that
        // is optimisation.
        MPM.run(*module, MAM);
    }

    void print_function(llvm::Module* module, std::string func_name) {
        verifyModule(*module, &llvm::outs());

        //Optimization setup based on https://github.com/bollu/simplexhc-cpp/blob/master/src/main.cpp

        PassBuilder PB;

        ModulePassManager MPM;
        FunctionPassManager FPM;
        CGSCCPassManager CGSCCPM;

        PassBuilder::OptimizationLevel optimisationLevel = PassBuilder::OptimizationLevel::O3;
        MPM = PB.buildModuleSimplificationPipeline(optimisationLevel, PassBuilder::ThinLTOPhase::None);
        FPM = PB.buildFunctionSimplificationPipeline(optimisationLevel, PassBuilder::ThinLTOPhase::None);

        LoopAnalysisManager LAM;
        FunctionAnalysisManager FAM;
        CGSCCAnalysisManager CGAM;
        ModuleAnalysisManager MAM;

        // Register the AA manager first so that our version is the one used.
        FAM.registerPass([&] { return PB.buildDefaultAAPipeline(); });

        PB.registerModuleAnalyses(MAM);
        PB.registerCGSCCAnalyses(CGAM);
        PB.registerFunctionAnalyses(FAM);
        PB.registerLoopAnalyses(LAM);
        PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

        // Fix the IR first, then run optimisations.
        MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
        MPM.addPass(createModuleToPostOrderCGSCCPassAdaptor(std::move(CGSCCPM)));

        //Print module IR
        //MPM.addPass(PrintModulePass(llvm::outs()));


        // We need to run the pipeline once for correctness. Anything after that
        // is optimisation.
        MPM.run(*module, MAM);

    }
}
