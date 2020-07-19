//
// Created by rob on 7/17/20.
//

#include "BlockCache.h"

#include <iostream>

namespace hacklift {
    BlockCache::BlockCache(LLVMContext &c, Function &f) : ctx(c), func(f) { }

    /*
    BasicBlock *BlockCache::operator[](const std::string &s) {
        if (blocks.find(s) != blocks.end()) {
            return blocks[s];
        } else {
            if (!locked) {
                auto it = blocks.emplace(s, BasicBlock::Create(ctx, s, &func));
                return it.first->second;
            } else {
                throw std::runtime_error("Block cache is locked, cannot create new block named: " + s);
            }
        }
    }
     */

    BasicBlock *BlockCache::operator[](std::string_view sv) {
        const std::string temp(sv);
        if (blocks.find(temp) != blocks.end()) {
            return blocks[temp];
        } else {
            if (!locked) {
                auto it = blocks.emplace(temp, BasicBlock::Create(ctx, temp, &func));
                return it.first->second;
            } else {
                throw std::runtime_error("Block cache is locked, cannot create new block named: " + temp);
            }
        }
    }

    void BlockCache::lock() {
        locked = true;
    }

}
