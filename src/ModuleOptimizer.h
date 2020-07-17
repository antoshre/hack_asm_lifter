//
// Created by rob on 7/16/2020.
//

#include "llvm/IR/IRBuilder.h"


#ifndef HACK_LIFTER_MODULEOPTIMIZER_H
#define HACK_LIFTER_MODULEOPTIMIZER_H

namespace hacklift {
        void optimize(llvm::Module*);

    void print_function(llvm::Module* module, std::string func_name);
}

#endif //HACK_LIFTER_MODULEOPTIMIZER_H
