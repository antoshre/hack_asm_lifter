//
// Created by rob on 7/18/2020.
//

#include "hacklift/BlockMap.h"

namespace hacklift {
    BasicBlock *BlockMap::operator[](int16_t v) {
        if (map.find(v) != map.end()) {
            return map[v];
        } else {
            throw std::runtime_error("BlockMap lookup failure");
        }
        return nullptr;
    }

    void BlockMap::insert(std::pair<int16_t, BasicBlock *> p) {
        map.insert(p);
    }

    void BlockMap::emplace(int16_t k, BasicBlock *v) {
        map.emplace(k, v);
    }
}
