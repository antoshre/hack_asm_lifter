//
// Created by rob on 7/16/2020.
//

#include "BuilderHelper.h"

namespace hacklift {
    BuilderHelper::BuilderHelper(IRBuilder<> &b, LLVMContext &c) : bldr(b), ctx(c) {}

    Value *BuilderHelper::get_constant(int val) {
        return llvm::ConstantInt::get(IntegerType::getInt32Ty(ctx), val);
    }

    Value *BuilderHelper::ptr_offset(Value *ptr, Value *offset) {
        return bldr.CreateGEP(ptr, offset);
    }

    Value *BuilderHelper::ptr_offset(Value *ptr, int offset) {
        auto c = get_constant(offset);
        return ptr_offset(ptr, c);
    }

    Value *BuilderHelper::deref(Value *ptr) {
        return bldr.CreateLoad(ptr);
    }

    Value *BuilderHelper::load_from(Value *ptr) {
        return deref(ptr);
    }

    Value *BuilderHelper::store_to(Value *ptr, Value *val) {
        return bldr.CreateStore(val, ptr);
    }

}