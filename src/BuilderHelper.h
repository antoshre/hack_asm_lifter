//
// Created by rob on 7/16/2020.
//

#ifndef HACK_LIFTER_BUILDERHELPER_H
#define HACK_LIFTER_BUILDERHELPER_H

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

using namespace llvm;

namespace hacklift {
    class BuilderHelper {
        IRBuilder<> &bldr;
        LLVMContext &ctx;

    public:
        BuilderHelper(IRBuilder<>& b, LLVMContext& c);

        //Convert integer constant into LLVM Value* constant
        Value* get_constant(int);
        //Access pointer + offset, but does NOT deref it!
        /*
         * int arr[64];
         * auto p = (arr + offset*sizeof(arr[0]))
         */
        Value* ptr_offset(Value*, Value*);
        Value* ptr_offset(Value*, int);
        Value* deref(Value*);

        Value* load_from(Value*);
        Value* store_to(Value*, Value*);

    };
}

#endif //HACK_LIFTER_BUILDERHELPER_H
