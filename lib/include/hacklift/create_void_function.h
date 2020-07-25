//
// Created by rob on 7/18/20.
//

#ifndef HACK_LIFTER_CREATE_VOID_FUNCTION_H
#define HACK_LIFTER_CREATE_VOID_FUNCTION_H

#include <string>
#include "llvm/IR/Module.h"

using namespace llvm;

namespace hacklift {
    void create_void_function(Module &, const std::string &);
}
#endif //HACK_LIFTER_CREATE_VOID_FUNCTION_H
