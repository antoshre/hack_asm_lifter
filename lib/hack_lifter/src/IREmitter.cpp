//
// Created by rob on 7/17/20.
//

#include "IREmitter.h"

namespace hacklift {
    /*
    IREmitter::IREmitter(ASTTransformer &a) : ast(a) {}

    void IREmitter::operator()(const hackasm::A_Type &i) {
        ast.state.A = ast.h->i16(i.s.value);
    }

    void IREmitter::operator()(const hackasm::L_Type &i) {
        throw std::runtime_error("Labels and jumps not implemented yet");
    }

    void IREmitter::operator()(const hackasm::C_Type &i) {
        if (i.dest_mnemonic != "null") {
            //TODO: clean up the lambda-ness and pointer mess
            std::unordered_map<std::string, Value *(*)(ASTTransformer *)> comp_values = {
                    {"0",   [](auto p) {
                        return p->h->i16(0);
                    }},
                    {"1",   [](auto p) {
                        return p->h->i16(1);
                    }},
                    {"-1",  [](auto p) {
                        return p->h->i16(-1);
                    }},
                    {"D",   [](auto p) {
                        return p->state.D;
                    }},
                    {"A",   [](auto p) {
                        return p->state.A;
                    }},
                    {"!D",  [](auto p) {
                        return p->h->op_not(p->state.D);
                    }},
                    {"-D",  [](auto p) {
                        return p->h->op_neg(p->state.D);
                    }},
                    {"-A",  [](auto p) {
                        return p->b->CreateNeg(p->state.A);
                    }},
                    {"D+1", [](auto p) {
                        return p->b->CreateAdd(p->state.D, p->h->i16(1));
                    }},
                    {"A+1", [](auto p) {
                        return p->b->CreateAdd(p->state.A, p->h->i16(1));
                    }},
                    {"D-1", [](auto p) {
                        return p->b->CreateSub(p->state.D, p->h->i16(1));
                    }},
                    {"A-1", [](auto p) {
                        return p->b->CreateSub(p->state.A, p->h->i16(1));
                    }},
                    {"D+A", [](auto p) {
                        return p->b->CreateAdd(p->state.D, p->state.A);
                    }},
                    {"D-A", [](auto p) {
                        return p->b->CreateSub(p->state.D, p->state.A);
                    }},
                    {"A-D", [](auto p) {
                        return p->b->CreateSub(p->state.A, p->state.D);
                    }},
                    {"D&A", [](auto p) {
                        return p->b->CreateAnd(p->state.D, p->state.A);
                    }},
                    {"D|A", [](auto p) {
                        return p->b->CreateOr(p->state.D, p->state.A);
                    }},
                    {"D|M", [](auto p) {
                        return p->b->CreateOr(p->state.D, p->h->read_array(p->state.M, p->state.A));
                    }},
                    {"D&M", [](auto p) {
                        return p->b->CreateAnd(p->state.D, p->h->read_array(p->state.M,
                                                                          p->state.A));
                    }},
                    {"M-D", [](auto p) {
                        return p->b->CreateSub(p->h->read_array(p->state.M, p->state.A),
                                               p->state.D);
                    }},
                    {"D-M", [](auto p) {
                        return p->b->CreateSub(p->state.D, p->h->read_array(p->state.M,
                                                                          p->state.A));
                    }},
                    {"D+M", [](auto p) {
                        return p->b->CreateAdd(p->state.D, p->h->read_array(p->state.M,
                                                                          p->state.A));
                    }},
                    {"M-1", [](auto p) {
                        return p->b->CreateSub(p->h->read_array(p->state.M, p->state.A),
                                               p->h->i16(-1));
                    }},
                    {"-M",  [](auto p) {
                        return p->b->CreateNeg(p->h->read_array(p->state.M, p->state.A));
                    }},
                    {"!M",  [](auto p) {
                        return p->b->CreateNot(p->h->read_array(p->state.M, p->state.A));
                    }},
                    {"M",   [](auto p) {
                        return p->h->read_array(p->state.M, p->state.A);
                    }}

            };
            Value *comp = nullptr;
            if (comp_values.find(i.comp_mnemonic) != comp_values.end()) {
                comp = comp_values[i.comp_mnemonic](&ast);
            } else {
                throw std::runtime_error("Unhandled comp mnemonic: " + i.comp_mnemonic);
            }

            Value **dest;
            if (i.dest_mnemonic.find('A') != std::string::npos) {
                //state.A = comp;
                dest = &ast.state.A;
            } else if (i.dest_mnemonic.find('D') != std::string::npos) {
                //state.D = comp;
                dest = &ast.state.D;
            } else if (i.dest_mnemonic.find('M') != std::string::npos) {
                //bhlp_ptr->write_elem(state.M, state.A, comp);
                auto temp = ast.h->write_array(ast.state.M, ast.state.A, comp);
                dest = &temp;
            } else {
                throw std::runtime_error("Unhandled dest mnemonic: " + i.dest_mnemonic);
            }
            *dest = comp;

        } else {
            //It's a jump-type
            //comp ; jump
            throw std::runtime_error("Jumps not implemented");
        }
    }
     */

    IREmitter::IREmitter(HackMachineState &_m, BlockMap &_b, BuilderHelper &_h) : h(_h), bmap(_b), m(_m) {}

    void IREmitter::operator()(const hackasm::A_Type &i) {
        m.A = h.i16(i.s.value);
    }

    void IREmitter::operator()(const hackasm::L_Type &i) {
        BasicBlock::iterator it = h.bldr.GetInsertPoint();
        llvm::Instruction *last_instruction = it->getPrevNode();
        if (auto *bi = dyn_cast<BranchInst>(last_instruction)) {
            //last instruction was a branch, don't insert another superfluous branch
            //BBs must only have a single terminator
            h.bldr.SetInsertPoint(h.bblocks[i.s.label]);
        } else {
            h.bldr.CreateBr(h.bblocks[i.s.label]);
            h.bldr.SetInsertPoint(h.bblocks[i.s.label]);
        }
    }

    void IREmitter::operator()(const hackasm::C_Type &i) {
        //All C-types have a comp section, so handle that first
        Value *comp = nullptr;
        if (comp_values.find(i.comp_mnemonic) != comp_values.end()) {
            comp = comp_values[i.comp_mnemonic](this);

        } else {
            throw std::runtime_error("Unhandled comp mnemonic: " + i.comp_mnemonic);
        }

        if (i.dest_mnemonic != "null") {
            //It's a regular computation.
            //ex: D=M+1 -> dest=D, comp=M+1
            //No jump to worry about
            Value *dest;
            if (i.dest_mnemonic.find('A') != std::string::npos) {
                //set register A to comp
                m.A = comp;
            } else if (i.dest_mnemonic.find('D') != std::string::npos) {
                //set register D to comp
                m.D = comp;
            } else if (i.dest_mnemonic.find('M') != std::string::npos) {
                //write comp to MEM[A}
                h.write_array(m.M, m.A, comp);
            } else {
                //How?
                throw std::runtime_error("Unhandled dest mnemonic: " + i.dest_mnemonic);
            }
        } else {
            //Oh lord it's a jump.
            //ex: D;JGT
            // comp=D, jump=JMP
            // if (D > 0): goto Label(A)

            //BasicBlock* target = bmap[m.A]; //look up target in Value -to- Label-BB map
            auto *cint = dyn_cast<ConstantInt>(m.A);
            int16_t val = cint->getSExtValue();
            std::cout << "Branch target is #" << val << '\n';
            BasicBlock *target = bmap[val];

            //if val == instruction location, it's a trivial infinite loop
            if (i.jump_mnemonic == "JMP" && i.inst_loc == val + 1) {
                //Trivial infinite loop detected,
                //throw std::runtime_error("Infinite loop detected, not currently handled");
                std::cout << "Infinite loop detected, ignoring...\n";
                //h.bldr.CreateRetVoid();
                return;
            }

            Value *jump;
            if (i.jump_mnemonic == "JGT") {
                h.JGT(comp, h.i16(0), target);
                return;
            }
            if (i.jump_mnemonic == "JEQ") {
                h.JEQ(comp, h.i16(0), target);
                return;
            }
            if (i.jump_mnemonic == "JGE") {
                h.JGE(comp, h.i16(0), target);
                return;
            }
            if (i.jump_mnemonic == "JLT") {
                h.JLT(comp, h.i16(0), target);
                return;
            }
            if (i.jump_mnemonic == "JNE") {
                h.JNE(comp, h.i16(0), target);
                return;
            }
            if (i.jump_mnemonic == "JLE") {
                h.JLE(comp, h.i16(0), target);
                return;
            }
            if (i.jump_mnemonic == "JMP") {
                h.JMP(comp, h.i16(0), target);
                return;
            }
        }
    }
}