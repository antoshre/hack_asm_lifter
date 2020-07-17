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
         * //C equivalent of:
         * auto ptr = (arr+offset);
         * //Math equivalent of:
         * auto p = (arr + offset*sizeof(arr[0]))
         */
        Value* ptr_offset(Value*, Value*);
        Value* ptr_offset(Value*, int);
        Value* deref(Value*);
        //Get array element
        Value* get_elem(Value*, int);
        Value* get_elem(Value*, Value*);

        //Write array element
        Value* write_elem(Value*, int, Value*);
        Value* write_elem(Value*, Value*, Value*);

        //same as deref
        Value* load_from(Value*);
        Value* store_to(Value*, Value*);

    };
}

#endif //HACK_LIFTER_BUILDERHELPER_H
