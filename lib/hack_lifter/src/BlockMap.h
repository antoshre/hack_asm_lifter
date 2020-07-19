//
// Created by rob on 7/18/2020.
//

#ifndef HACK_LIFTER_BLOCKMAP_H
#define HACK_LIFTER_BLOCKMAP_H

#include "llvm/IR/Value.h"
#include "llvm/IR/BasicBlock.h"
#include <unordered_map>

using namespace llvm;

namespace hacklift {
    //Maps Value* labels to BasicBlocks.
    class BlockMap {
        std::unordered_map<int16_t, BasicBlock *> map;

    public:
        BlockMap() = default;

        void insert(std::pair<int16_t, BasicBlock *>);

        void emplace(int16_t, BasicBlock *);

        BasicBlock *operator[](int16_t);
    };
}

#endif //HACK_LIFTER_BLOCKMAP_H
