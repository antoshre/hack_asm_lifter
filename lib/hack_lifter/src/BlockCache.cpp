//
// Created by rob on 7/17/20.
//

#include "BlockCache.h"

#include <iostream>

namespace hacklift {
    BlockCache::BlockCache(LLVMContext &c, Function &f) : ctx(c), func(f) { }

    BasicBlock *BlockCache::operator[](const std::string &s) {
        if (blocks.find(s) != blocks.end()) {
            std::cout << "[BlockCache] BBlock found: " << s << '\n';
            return blocks[s];
        } else {
            if (!locked) {
                std::cout << "[BlockCache] BBlock added: " << s << '\n';
                auto it = blocks.emplace(s, BasicBlock::Create(ctx, s, &func));
                return it.first->second;
            } else {
                throw std::runtime_error("Block cache is locked, cannot create new block named: " + s);
            }
        }
    }

    void BlockCache::lock() {
        locked = true;
    }

}
