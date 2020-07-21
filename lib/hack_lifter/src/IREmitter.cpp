//
// Created by rob on 7/17/20.
//

#include <llvm/Support/raw_ostream.h>
#include "IREmitter.h"

namespace hacklift {

    IREmitter::IREmitter(HackMachineState &_m, BlockMap &_b, BuilderHelper &_h) : h(_h), bmap(_b), m(_m) {}

    void IREmitter::operator()(const hackasm::A_Type &i) {
        auto c = h.i16(i.s.value);
        m.A = c;
        //h.bldr.CreateLoad(m.A);
        //h.bldr.CreateStore(c, m.A);
        //m.A = h.i16(i.s.value);
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
        std::string comp_str(i.comp_mnemonic);
        if (comp_values.find(comp_str) != comp_values.end()) {
            comp = comp_values[comp_str](this);

        } else {
            throw std::runtime_error("Unhandled comp mnemonic: " + comp_str);
        }

        if (i.dest_mnemonic != "null") {
            //It's a regular computation.
            //ex: D=M+1 -> dest=D, comp=M+1
            //No jump to worry about
            Value *dest;
            if (i.dest_mnemonic.find('A') != std::string::npos) {
                //set register A to comp
                //auto temp = h.bldr.CreateLoad(m.A);
                //h.bldr.CreateStore(comp, m.A);
                m.A = comp;
            } else if (i.dest_mnemonic.find('D') != std::string::npos) {
                //set register D to comp
                //auto temp = h.bldr.CreateLoad(m.D);
                //auto temp2 = h.bldr.CreateStore(comp, temp);
                h.bldr.CreateStore(comp, m.D);
                //m.D = comp;
            } else if (i.dest_mnemonic.find('M') != std::string::npos) {
                //write comp to MEM[A}
                //auto temp = m.M = h.read_array(m.MEM, m.A);
                //Value *A = h.bldr.CreateLoad(m.A);
                h.write_array(m.MEM, m.A, comp);
            } else {
                //How?
                throw std::runtime_error("Unhandled dest mnemonic: " + std::string(i.dest_mnemonic));
            }
        } else {
            //Oh lord it's a jump.
            //ex: D;JGT
            // comp=D, jump=JMP
            // if (D > 0): goto Label(A)

            //BasicBlock* target = bmap[m.A]; //look up target in Value -to- Label-BB map
            //Value* temp = h.bldr.CreateLoad(m.A)->getPointerOperand();
            //Value *temp = h.bldr.CreateLoad(m.A);
            Value *temp = m.A;
            temp->print(llvm::outs(), true);
            llvm::outs() << temp->getName() << '\n';
            int16_t val;
            if (isa<ConstantInt>(temp)) {
                auto *cint = dyn_cast<ConstantInt>(temp);
                val = cint->getSExtValue();
            } else {
                throw std::runtime_error("A isn't a ConstantInt");
            }

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