//
// Created by rob on 7/18/20.
//

#include "hacklift/create_void_function.h"

#include "hacklift/BlockCache.h"
#include "hacklift/BuilderHelper.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include <iostream>
#include "hacklift/HackMachineState.h"

namespace hacklift {

    extern "C" void dump_mem(int16_t *mem) {
        std::cout << "dump_mem:\n";
    }

    void create_void_function(Module &module, const std::string &fname) {
        auto &ctx = module.getContext();
        auto func = module.getOrInsertFunction(fname, IntegerType::getVoidTy(ctx), IntegerType::getInt16PtrTy(ctx));
        auto foo = cast<Function>(func.getCallee());

        BlockCache bblocks(ctx, *foo);
        IRBuilder<> b(ctx);
        BuilderHelper h(b, ctx, bblocks);

        HackMachineState state{};

        auto foo_args = foo->arg_begin();
        state.MEM = foo_args++;
        state.MEM->setName("MEM");

        b.SetInsertPoint(bblocks["entry"]);

        //Input: M[0], M[1]
        //Return: max(M[0], M[1])
        /*
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
        */

        bblocks["DGREATER"];
        bblocks["EXIT"];
        //M[2] = M[0] + M[1]
        state.A = h.i16(0);
        state.D = h.read_array(state.MEM, state.A);
        state.A = h.i16(1);
        state.D = h.op_sub(state.D, h.read_array(state.MEM, state.A));
        h.JGT(state.D, h.i16(0), bblocks["DGREATER"]);
        state.A = h.i16(1);
        state.D = h.read_array(state.MEM, state.A);
        state.A = h.i16(2);
        //state.D = h.read_array(state.M, state.A);
        h.write_array(state.MEM, state.A, state.D);
        state.A = h.i16(17);
        h.JMP(bblocks["EXIT"]);

        b.SetInsertPoint(bblocks["DGREATER"]);
        state.A = h.i16(0);
        state.D = h.read_array(state.MEM, state.A);
        state.A = h.i16(2);
        h.write_array(state.MEM, state.A, state.D);
        state.A = h.i16(17);
        b.CreateBr(bblocks["EXIT"]);

        b.SetInsertPoint(bblocks["EXIT"]);

        //auto keyboard_func_ptr = module.getOrInsertFunction("handle_keyboard", IntegerType::getVoidTy(ctx), IntegerType::getInt16PtrTy(ctx));
        //auto keyboard_func = cast<Function>(keyboard_func_ptr.getCallee());

        //handle_keyboard();

        ArrayRef<Value *> args = {state.MEM};
        //b.CreateCall(keyboard_func, args);
        b.CreateRetVoid();


    }
}