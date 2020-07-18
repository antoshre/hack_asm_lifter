//
// Created by rob on 7/16/2020.
//

#ifndef HACK_LIFTER_BUILDERHELPER_H
#define HACK_LIFTER_BUILDERHELPER_H

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "BlockCache.h"

using namespace llvm;

namespace hacklift {
    class BuilderHelper {
        IRBuilder<> &bldr;
        LLVMContext &ctx;
        BlockCache &bblocks;

        void jump_if(Value*, BasicBlock*);
    public:
        BuilderHelper(IRBuilder<>& b, LLVMContext& c, BlockCache& bc);

        //Create an i16 constant
        Value *i16(int);

        Value *write_array(Value*, int, Value*);
        Value *write_array(Value*, Value*, Value*);
        Value *read_array(Value*, int);
        Value *read_array(Value*, Value*);

        //All operations are signed i16 binary ops
        void JGT(Value*, Value*, BasicBlock*);
        void JEQ(Value*, Value*, BasicBlock*);
        void JGE(Value*, Value*, BasicBlock*);
        void JLT(Value*, Value*, BasicBlock*);
        void JNE(Value*, Value*, BasicBlock*);
        void JLE(Value*, Value*, BasicBlock*);
        void JMP(Value*, Value*, BasicBlock*);
        void JMP(BasicBlock*);

        Value *op_add(Value*, Value*);
        Value *op_sub(Value*, Value*);
        Value *op_not(Value*);
        Value *op_neg(Value*);
    };
}

#endif //HACK_LIFTER_BUILDERHELPER_H
