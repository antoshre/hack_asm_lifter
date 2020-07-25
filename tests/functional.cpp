//
// Created by rob on 7/24/20.
//

#include "gtest/gtest.h"

#include "hacklift/parse_asm.h"
#include "hacklift/ModuleTools.h"

#include <fstream>
#include <array>

TEST(AddAsm, Functionality) {
    //Parse, recompile, and run Add.asm.
    //Check that M[2] = M[0] + M[1], no other change in memory
    std::ifstream file("../../examples/Add.asm", std::ios::in);

    hackasm::AST ast(file);

    auto ctx = std::make_unique<llvm::LLVMContext>();
    auto mod = std::make_unique<llvm::Module>("module", *ctx);

    hacklift::parse_asm_file(*mod, ast);

    ASSERT_NO_THROW(hacklift::verify_module(*mod));

    std::array<int16_t, 32768> mem{0};
    mem[0] = 50; //Chosen at random
    mem[1] = 85; //Chosen at random

    std::array<int16_t, 32768> original{0};
    std::copy(mem.begin(), mem.end(), original.begin());

    hacklift::run_void_func(std::move(mod), std::move(ctx), "f", mem, false);

    //No changes to arguments
    EXPECT_EQ(mem[0], original[0]);
    EXPECT_EQ(mem[1], original[1]);
    //MEM[2] = MEM[0] + MEM[1]
    EXPECT_EQ(original[0] + original[1], mem[2]);
    //No other changes to MEM
    for (int i = 3; i < mem.size(); i++) {
        EXPECT_EQ(mem[i], original[i]);
    }
}
