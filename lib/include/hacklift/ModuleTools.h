//
// Created by rob on 7/16/2020.
//

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"


#ifndef HACK_LIFTER_MODULEOPTIMIZER_H
#define HACK_LIFTER_MODULEOPTIMIZER_H

using namespace llvm;

namespace hacklift {
    void optimize_module(llvm::Module &);

    void print_module(llvm::Module &);

    void verify_module(llvm::Module &);

    int16_t run_int16_func(std::unique_ptr<Module>, std::unique_ptr<LLVMContext>, const std::string &,
                           std::array<int16_t, 32768> &, bool= true);

    void run_void_func(std::unique_ptr<Module>, std::unique_ptr<LLVMContext>, const std::string &,
                       std::array<int16_t, 32768> &, bool= true);
    //int run(std::unique_ptr<Module>&, const std::string& func_name, std::array<int16_t, 16>&);
}

#endif //HACK_LIFTER_MODULEOPTIMIZER_H
