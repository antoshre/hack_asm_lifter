//
// Created by rob on 7/18/2020.
//
#include "parse_asm.h"
#include "BlockCache.h"
#include "HackMachineState.h"
#include "BuilderHelper.h"
#include "IREmitter.h"
#include "llvm/IR/IRBuilder.h"


using namespace llvm;

namespace hacklift {
    void parse_asm_file(llvm::Module &module, const hackasm::AST &ast) {
        auto &ctx = module.getContext();
        auto func = module.getOrInsertFunction("f", IntegerType::getVoidTy(ctx), IntegerType::getInt16PtrTy(ctx));
        auto foo = cast<Function>(func.getCallee());

        BlockCache bblocks(ctx, *foo);
        BlockMap blockmap{};
        IRBuilder<> b(ctx);
        BuilderHelper h(b, ctx, bblocks);

        b.SetInsertPoint(bblocks["entry"]);

        HackMachineState state{};

        auto foo_args = foo->arg_begin();
        state.MEM = foo_args++;
        state.MEM->setName("MEM");

        //state.A = b.CreateAlloca(IntegerType::getInt16Ty(ctx), nullptr, "A");
        //state.A = h.i16(0);
        state.D = b.CreateAlloca(IntegerType::getInt16Ty(ctx), nullptr, "D");
        state.M = b.CreateAlloca(IntegerType::getInt16Ty(ctx), nullptr, "M");

        auto symbols = ast.get_symbol_table();

        for (const auto&[label, loc] : symbols.get_labels()) {
            //Create BB for every label
            bblocks[label];
            //Associate that BB with the label instruction location
            blockmap.emplace(loc, bblocks[label]);
        }

        IREmitter emitter(state, blockmap, h);
        for (const hackasm::Instruction &inst : ast.get_listing()) {
            std::visit(emitter, inst);
        }
        b.CreateRetVoid();
    }
}