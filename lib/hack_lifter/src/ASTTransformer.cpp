//
// Created by rob on 7/16/2020.
//

#include "ASTTransformer.h"

#include <utility>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/AssemblyAnnotationWriter.h>

namespace hacklift {
    ASTTransformer::ASTTransformer(hackasm::AST a) : ast(std::move(a)) {
        ctx = std::make_unique<LLVMContext>();
        module = std::make_unique<Module>("module_name", *ctx);
        //void(*)(int16_t[])
        func = module->getOrInsertFunction("foo",
                                           IntegerType::getVoidTy(*ctx),
                                           IntegerType::getInt16PtrTy(*ctx)
        );
        auto foo = cast<Function>(func.getCallee());
        auto args = foo->arg_begin();
        state.M = args;
        state.M->setName("Mem");

        bblocks.insert({"entry", BasicBlock::Create(*ctx, "entry", foo)});
        bldr_ptr = std::make_unique<IRBuilder<>>(bblocks["entry"]);
        bhlp_ptr = std::make_unique<BuilderHelper>(*bldr_ptr, *ctx);

        //Allocate stack space for A and D registers
        state.A = bldr_ptr->CreateAlloca(IntegerType::getInt16Ty(*ctx));
        state.A->setName("A");
        state.D = bldr_ptr->CreateAlloca(IntegerType::getInt16Ty(*ctx));
        state.D->setName("D");

        /*
         * //Add R0 to R1 and store in R2
         * @R0 //A=0            state.A = get_constant(0);
         * D=M //D=M[A]         state.D = get_elem(state.M, state.A)
         * @R1 //A=1            state.A = get_constant(1);
         * A=M //A=M[1]         state.A = get_elem(state.M, state.A)
         * D=D+A //D=M[0]+M[1]  state.D = CreateAdd(state.D, state.A)
         * @R2 //A=2            state.A = get_constant(2)
         * M=D //M[2]=D         auto slot = ptr_offset(state.M, state.A); CreateStore(slot, state.D)
         */
        /*
        state.A = bhlp_ptr->get_constant(0);
        state.D = bhlp_ptr->get_elem(state.M, state.A);
        state.A = bhlp_ptr->get_constant(1);
        state.A = bhlp_ptr->get_elem(state.M, state.A);
        state.D = bldr_ptr->CreateAdd(state.D, state.A);
        state.A = bhlp_ptr->get_constant(2);
        bhlp_ptr->write_elem(state.M, 2, state.D);
        */


        for (const hackasm::Instruction &i : ast.listing) {
            //A-Type instructions
            if (std::holds_alternative<hackasm::A_Type>(i)) {
                auto &inst = std::get<hackasm::A_Type>(i);
                state.A = bhlp_ptr->get_constant(inst.s.value);
            }
            //L-Type instructions
            if (std::holds_alternative<hackasm::L_Type>(i)) {
                throw std::runtime_error("L-Types not handled yet");
            }
            //C-Type instructions
            if (std::holds_alternative<hackasm::C_Type>(i)) {
                auto &inst = std::get<hackasm::C_Type>(i);

                if (inst.dest_mnemonic != "null") {
                    //It's a standard calculation
                    //dest = comp
                    /*
                     * This deserves some explanation.
                     * I want a data structure to hold this clearly-structured data,
                     * but the value elements must be evaluated at lookup.
                     */
                    std::unordered_map<std::string, Value *(*)(decltype(this))> comp_values = {
                            {"0",   [](auto p) { return p->bhlp_ptr->get_constant(0); }},
                            {"1",   [](auto p) { return p->bhlp_ptr->get_constant(1); }},
                            {"-1",  [](auto p) { return p->bhlp_ptr->get_constant(-1); }},
                            {"D",   [](auto p) { return p->state.D; }},
                            {"A",   [](auto p) { return p->state.A; }},
                            {"!D",  [](auto p) { return p->bldr_ptr->CreateNot(p->state.D); }},
                            {"-D",  [](auto p) { return p->bldr_ptr->CreateNeg(p->state.D); }},
                            {"-A",  [](auto p) { return p->bldr_ptr->CreateNeg(p->state.A); }},
                            {"D+1", [](auto p) {
                                return p->bldr_ptr->CreateAdd(p->state.D, p->bhlp_ptr->get_constant(1));
                            }},
                            {"A+1", [](auto p) {
                                return p->bldr_ptr->CreateAdd(p->state.A, p->bhlp_ptr->get_constant(1));
                            }},
                            {"D-1", [](auto p) {
                                return p->bldr_ptr->CreateSub(p->state.D, p->bhlp_ptr->get_constant(1));
                            }},
                            {"A-1", [](auto p) {
                                return p->bldr_ptr->CreateSub(p->state.A, p->bhlp_ptr->get_constant(1));
                            }},
                            {"D+A", [](auto p) { return p->bldr_ptr->CreateAdd(p->state.D, p->state.A); }},
                            {"D-A", [](auto p) { return p->bldr_ptr->CreateSub(p->state.D, p->state.A); }},
                            {"A-D", [](auto p) { return p->bldr_ptr->CreateSub(p->state.A, p->state.D); }},
                            {"D&A", [](auto p) { return p->bldr_ptr->CreateAnd(p->state.D, p->state.A); }},
                            {"D|A", [](auto p) { return p->bldr_ptr->CreateOr(p->state.D, p->state.A); }},
                            {"D|M", [](auto p) {
                                return p->bldr_ptr->CreateOr(p->state.D, p->bhlp_ptr->get_elem(p->state.M, p->state.A));
                            }},
                            {"D&M", [](auto p) {
                                return p->bldr_ptr->CreateAnd(p->state.D, p->bhlp_ptr->get_elem(p->state.M,
                                                                                                p->state.A));
                            }},
                            {"M-D", [](auto p) {
                                return p->bldr_ptr->CreateSub(p->bhlp_ptr->get_elem(p->state.M, p->state.A),
                                                              p->state.D);
                            }},
                            {"D-M", [](auto p) {
                                return p->bldr_ptr->CreateSub(p->state.D, p->bhlp_ptr->get_elem(p->state.M,
                                                                                                p->state.A));
                            }},
                            {"D+M", [](auto p) {
                                return p->bldr_ptr->CreateAdd(p->state.D, p->bhlp_ptr->get_elem(p->state.M,
                                                                                                p->state.A));
                            }},
                            {"M-1", [](auto p) {
                                return p->bldr_ptr->CreateSub(p->bhlp_ptr->get_elem(p->state.M, p->state.A),
                                                              p->bhlp_ptr->get_constant(-1));
                            }},
                            {"-M",  [](auto p) {
                                return p->bldr_ptr->CreateNeg(p->bhlp_ptr->get_elem(p->state.M, p->state.A));
                            }},
                            {"!M",  [](auto p) {
                                return p->bldr_ptr->CreateNot(p->bhlp_ptr->get_elem(p->state.M, p->state.A));
                            }},
                            {"M",   [](auto p) { return p->bhlp_ptr->get_elem(p->state.M, p->state.A); }}

                    };
                    Value *comp = nullptr;
                    if (comp_values.find(inst.comp_mnemonic) != comp_values.end()) {
                        comp = comp_values[inst.comp_mnemonic](this);
                    } else {
                        throw std::runtime_error("Unhandled comp mnemonic: " + inst.comp_mnemonic);
                    }

                    Value** dest;
                    if (inst.dest_mnemonic.find("A") != std::string::npos) {
                        //state.A = comp;
                        dest = &state.A;
                    } else if (inst.dest_mnemonic.find("D") != std::string::npos) {
                        //state.D = comp;
                        dest = &state.D;
                    } else if (inst.dest_mnemonic.find("M") != std::string::npos) {
                        //bhlp_ptr->write_elem(state.M, state.A, comp);
                        auto temp = bhlp_ptr->write_elem(state.M, state.A, comp);
                        dest = &temp;
                    } else {
                        throw std::runtime_error("Unhandled dest mnemonic: " + inst.dest_mnemonic);
                    }
                    *dest = comp;

                } else {
                    //It's a jump-type
                    //comp ; jump
                    throw std::runtime_error("Jumps not implemented");
                }
            }
        }
        //Final return from function
        bldr_ptr->CreateRetVoid();
    }

    std::string ASTTransformer::to_llvm_ir() {
        std::string out;
        llvm::raw_string_ostream sstream(out);
        AssemblyAnnotationWriter aaw;
        this->module->print(sstream, &aaw);
        return out;
    }
}
