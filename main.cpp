#include <iostream>
#include <memory>
#include <fstream>

#include "hackasm/AST.h"

#include "src/ASTTransformer.h"
#include "examples/simple_add_mod.h"
#include "src/ModuleTools.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"

using namespace llvm;

int main() {
    std::ifstream file("../examples/Add.asm", std::ios::in);
    hackasm::AST ast{hackasm::AsmFile(file)};

    hacklift::ASTTransformer transformer(ast);
    hacklift::verify_module(transformer.module.get());
    std::cout << "\nBefore optimization:\n" << std::endl;
    hacklift::print_module(transformer.module.get());
    hacklift::optimize_module(transformer.module.get());
    std::cout << "After optimization:\n" << std::endl;
    hacklift::print_module(transformer.module.get());

    hacklift::run(transformer.module, transformer.ctx, "foo");
    /*
    auto ctx = std::make_unique<LLVMContext>();
    auto module = std::unique_ptr<Module>(simple_add_module(*ctx, "test", "foo"));
    verifyModule(*module, &llvm::outs());

    std::cout << "Before optimization:" << std::endl;
    hacklift::print_module(module.get());
    hacklift::optimize_module(module.get());
    std::cout << "\n\nAfter optimization:" << std::endl;
    hacklift::print_module(module.get());
     */
    return 0;
}
