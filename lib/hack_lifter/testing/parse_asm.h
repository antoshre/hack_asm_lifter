//
// Created by rob on 7/18/2020.
//

#ifndef HACK_LIFTER_PARSE_ASM_H
#define HACK_LIFTER_PARSE_ASM_H

#include "hackasm/AST.h"
#include "llvm/IR/Module.h"

namespace hacklift {
    void parse_asm_file(llvm::Module &, const hackasm::AST &);
}

#endif //HACK_LIFTER_PARSE_ASM_H
