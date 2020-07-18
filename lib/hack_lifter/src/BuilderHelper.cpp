//
// Created by rob on 7/16/2020.
//

#include "BuilderHelper.h"

namespace hacklift {
    BuilderHelper::BuilderHelper(IRBuilder<> &b, LLVMContext &c, BlockCache& bc) : bldr(b), ctx(c), bblocks(bc) {}

    void BuilderHelper::jump_if(Value *cond, BasicBlock *target) {
        //Branches require two BB targets: true path and false path
        //Automatically create the false path with a unique name
        std::string false_branch_name = bldr.GetInsertBlock()->getName().str() + "_fallthrough";
        auto bb_after_cond = bblocks[false_branch_name];
        bldr.CreateCondBr(cond, target, bb_after_cond);
        //Setup builder to keep inserting after the branch on the false path
        bldr.SetInsertPoint(bb_after_cond);
    }

    Value *BuilderHelper::i16(int val) {
        return llvm::ConstantInt::get(IntegerType::getInt16Ty(ctx), val);
    }

    Value *BuilderHelper::write_array(Value *ptr, int offset, Value *val) {
        return write_array(ptr, i16(offset), val);
    }
    Value *BuilderHelper::write_array(Value *ptr, Value *offset, Value *val) {
        auto elem = bldr.CreateGEP(ptr, offset);
        return bldr.CreateStore(elem, val);
    }

    Value *BuilderHelper::read_array(Value *ptr, int offset) {
        return read_array(ptr, i16(offset));

    }

    Value *BuilderHelper::read_array(Value *ptr, Value *offset) {
        auto elem = bldr.CreateGEP(ptr, offset);
        return bldr.CreateLoad(elem);
    }

    //Branch to bb if lhs > rhs
    void BuilderHelper::JGT(Value *lhs, Value *rhs, BasicBlock *bb) {
        auto comp = bldr.CreateICmpSGT(lhs, rhs);
        jump_if(comp, bb);
    }

    void BuilderHelper::JEQ(Value *lhs, Value *rhs, BasicBlock *bb) {
        auto comp = bldr.CreateICmpEQ(lhs, rhs);
        jump_if(comp, bb);
    }

    void BuilderHelper::JGE(Value *lhs, Value *rhs, BasicBlock *bb) {
        auto comp = bldr.CreateICmpSGE(lhs, rhs);
        jump_if(comp, bb);
    }

    void BuilderHelper::JLT(Value *lhs, Value *rhs, BasicBlock *bb) {
        auto comp = bldr.CreateICmpSLT(lhs, rhs);
        jump_if(comp, bb);
    }

    void BuilderHelper::JNE(Value *lhs, Value *rhs, BasicBlock *bb) {
        auto comp = bldr.CreateICmpNE(lhs, rhs);
        jump_if(comp, bb);
    }

    void BuilderHelper::JLE(Value *lhs, Value *rhs, BasicBlock *bb) {
        auto comp = bldr.CreateICmpSLE(lhs, rhs);
        jump_if(comp, bb);
    }

    void BuilderHelper::JMP(Value *lhs, Value *rhs, BasicBlock *bb) {
        JMP(bb);
    }

    void BuilderHelper::JMP(BasicBlock *bb) {
        bldr.CreateBr(bb);
    }

    Value *BuilderHelper::op_add(Value *lhs, Value *rhs) {
        return bldr.CreateAdd(lhs, rhs);
    }

    Value *BuilderHelper::op_sub(Value *lhs, Value *rhs) {
        return bldr.CreateSub(lhs, rhs);
    }

    Value *BuilderHelper::op_not(Value *lhs) {
        return bldr.CreateNot(lhs);
    }

    Value *BuilderHelper::op_neg(Value *lhs) {
        return bldr.CreateNeg(lhs);
    }


}