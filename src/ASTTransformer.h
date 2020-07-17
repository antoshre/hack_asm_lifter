//
// Created by rob on 7/16/2020.
//

#ifndef HACK_LIFTER_ASTTRANSFORMER_H
#define HACK_LIFTER_ASTTRANSFORMER_H

#include "hackasm/AST.h"

namespace hacklift {
    class ASTTransformer {
        const hackasm::AST ast;
    public:
        explicit ASTTransformer(const hackasm::AST&);
    };
}

#endif //HACK_LIFTER_ASTTRANSFORMER_H
