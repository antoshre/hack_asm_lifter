//
// Created by rob on 7/17/20.
//

#ifndef HACK_LIFTER_BLOCKCACHE_H
#define HACK_LIFTER_BLOCKCACHE_H

#include <string>
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Function.h"

using namespace llvm;

namespace hacklift {
    class BlockCache {
        std::unordered_map<std::string, BasicBlock *> blocks{};
        LLVMContext &ctx;
        Function &func;
        bool locked = false;


    public:
        BlockCache(LLVMContext &, Function &);

        //BasicBlock* operator[](const std::string&);
        BasicBlock *operator[](std::string_view);

        //Prevent the creation of new basic blocks.
        void lock();
    };
}

#endif //HACK_LIFTER_BLOCKCACHE_H
