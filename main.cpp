#include <iostream>
#include <memory>

#include "examples/simple_add_mod.h"
#include "src/ModuleOptimizer.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"

using namespace llvm;

int main() {
    auto ctx = std::make_unique<LLVMContext>();
    auto module = std::unique_ptr<Module>(simple_add_module(*ctx, "test", "foo"));

    verifyModule(*module, &llvm::outs());

    std::cout << "Before optimization:";
    //hacklift::print_function(module.get(), "foo");
    hacklift::optimize(module.get());
    std::cout << "\n\nAfter optimization:";
    //hacklift::print_function(module.get(), "foo");
    return 0;
}
