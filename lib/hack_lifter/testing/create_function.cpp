//
// Created by rob on 7/18/20.
//

#include "create_function.h"

#include "BlockCache.h"
#include "BuilderHelper.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include <iostream>

namespace hacklift {

    extern "C" void dump_mem(int16_t *mem) {
        std::cout << "dump_mem:\n";
    }
    void create_function(Module &module, const std::string& fname) {
        auto& ctx = module.getContext();
        auto func = module.getOrInsertFunction(fname, IntegerType::getInt16Ty(ctx), IntegerType::getInt16PtrTy(ctx));
        auto foo = cast<Function>(func.getCallee());

        BlockCache bblocks(ctx, *foo);
        IRBuilder<> b(ctx);
        BuilderHelper h(b, ctx, bblocks);

        struct {
            Value *A,*D,*M;
        } state{};

        auto foo_args = foo->arg_begin();
        state.M = foo_args++;
        state.M->setName("M");

        b.SetInsertPoint(bblocks["entry"]);

        state.A = h.i16(0);
        state.D = h.read_array(state.M, state.A);
        state.A = h.i16(1);
        state.A = h.read_array(state.M, state.A);
        //state.D = b.CreateSub(state.D, state.A);
        state.D = h.op_sub(state.D, state.A);
        //auto cond = b.CreateICmpSGT(state.D, h.i16(0));
        //b.CreateCondBr( cond, retmo, fallthrough);
        h.JGT(state.D, h.i16(0), bblocks["retmo"]);
        state.A = h.i16(1);
        state.D = h.read_array(state.M, state.A);
        b.CreateRet(state.D);

        b.SetInsertPoint(bblocks["retmo"]);
        state.A = h.i16(0);
        state.D = h.read_array(state.M, state.A);
        b.CreateRet(state.D);
    }
}