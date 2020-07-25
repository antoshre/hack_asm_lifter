//
// Created by rob on 7/18/2020.
//
#include "hacklift/parse_asm.h"
#include "hacklift/BlockCache.h"
#include "hacklift/HackMachineState.h"
#include "hacklift/BuilderHelper.h"
#include "hacklift/IREmitter.h"
#include "llvm/IR/IRBuilder.h"


using namespace llvm;

namespace hacklift {
    void parse_asm_file(llvm::Module &module, const hackasm::AST &ast) {
        auto &ctx = module.getContext();
        auto func = module.getOrInsertFunction("f", IntegerType::getVoidTy(ctx), IntegerType::getInt16PtrTy(ctx));
        auto foo = cast<Function>(func.getCallee());

        //Initial support for keyboard and screen instrumentation
        auto keyboard_func = module.getOrInsertFunction("handle_keyboard", IntegerType::getVoidTy(ctx),
                                                        IntegerType::getInt16PtrTy(ctx));
        auto keyboard = cast<Function>(keyboard_func.getCallee());
        auto screen_func = module.getOrInsertFunction("handle_screen", IntegerType::getVoidTy(ctx),
                                                      IntegerType::getInt16PtrTy(ctx));
        auto screen = cast<Function>(keyboard_func.getCallee());

        BlockCache bblocks(ctx, *foo);
        BlockMap blockmap{};
        IRBuilder<> b(ctx);
        BuilderHelper h(b, ctx, bblocks);

        b.SetInsertPoint(bblocks["entry"]);

        HackMachineState state{};
        state.keyboard_update = keyboard;
        state.screen_update = screen;

        auto foo_args = foo->arg_begin();
        state.MEM = foo_args++;
        state.MEM->setName("MEM");

        //state.A = b.CreateAlloca(IntegerType::getInt16Ty(ctx), nullptr, "A");
        //state.A = h.i16(0);
        state.D = b.CreateAlloca(IntegerType::getInt16Ty(ctx), nullptr, "D");

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