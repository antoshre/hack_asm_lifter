//
// Created by rob on 7/17/20.
//

#include <fstream>

#include "hackasm/AsmFile.h"
#include "hackasm/AST.h"

#include "ASTTransformer.h"

int main() {
    std::ifstream file("../../examples/Add.asm", std::ios::in);

    hackasm::AsmFile asmfile(file);
    hackasm::AST ast(asmfile);

    hacklift::ASTTransformer transformer(ast);

    std::cout << transformer.to_llvm_ir() << std::endl;

    return 0;
}
