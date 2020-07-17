//
// Created by rob on 7/16/2020.
//

#ifndef HACK_LIFTER_SIMPLE_ADD_MOD_H
#define HACK_LIFTER_SIMPLE_ADD_MOD_H

#include "../src/BuilderHelper.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/IRPrintingPasses.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"

#include "llvm/ExecutionEngine/ORC/LLJIT.h"

#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/Bitcode/BitcodeWriter.h"

using namespace llvm;

/*
 * Module modeling:
 * void foo(int16 Mem*) {
 *      Mem[2] = Mem[0] + Mem[1];
 * }
 */
inline Module* simple_add_module(LLVMContext& llvm_ctx, std::string module_name, std::string func_name) {
    auto mod = new Module(module_name, llvm_ctx);
    auto foo_func = mod->getOrInsertFunction(func_name,
                                             IntegerType::getVoidTy(llvm_ctx), //returns void
                                             IntegerType::getInt16PtrTy(llvm_ctx) //accepts int16[] as only param
    );

    auto foo = cast<Function>(foo_func.getCallee());
    auto args = foo->arg_begin();
    Value *Mem = args++;
    Mem->setName("Mem");

    auto entry = BasicBlock::Create(llvm_ctx, "entry", foo);
    IRBuilder<> builder(entry);
    hacklift::BuilderHelper bhlp(builder, llvm_ctx);

    /*
    auto R0 = bhlp.read_offset(Mem, 0);
    auto R1 = bhlp.read_offset(Mem, 1);
    auto res = builder.CreateAdd(R0, R1);
    bhlp.write_offset(Mem, res, 2);
    */
    auto R0 = bhlp.deref(bhlp.ptr_offset(Mem, 0));
    auto R1 = bhlp.deref(bhlp.ptr_offset(Mem, 1));
    auto res = builder.CreateAdd(R0, R1);
    auto _ = builder.CreateSub(R0, R1);
    bhlp.store_to( bhlp.ptr_offset(Mem, 2), res);

    builder.CreateRetVoid();

    verifyFunction(*mod->getFunction(func_name), &llvm::outs());
    return mod;
}

#endif //HACK_LIFTER_SIMPLE_ADD_MOD_H
