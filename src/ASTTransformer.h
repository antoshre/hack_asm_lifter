//
// Created by rob on 7/16/2020.
//

#ifndef HACK_LIFTER_ASTTRANSFORMER_H
#define HACK_LIFTER_ASTTRANSFORMER_H

#include "hackasm/AST.h"
#include "BuilderHelper.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace hacklift {

    struct HackMachineState {
        Value *A,*D,*M;
    };
    struct ASTTransformer {
        HackMachineState state;

        const hackasm::AST ast;
        std::unique_ptr<LLVMContext> ctx;
        std::unique_ptr<IRBuilder<>> bldr_ptr;
        std::unique_ptr<BuilderHelper> bhlp_ptr;

        std::unique_ptr<Module> module;
        FunctionCallee func;

        std::unordered_map<std::string, BasicBlock*> bblocks;
    public:
        explicit ASTTransformer(hackasm::AST );

        ~ASTTransformer();
    };
}

#endif //HACK_LIFTER_ASTTRANSFORMER_H
