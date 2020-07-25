//
// Created by rob on 7/17/20.
//

#ifndef HACK_LIFTER_IREMITTER_H
#define HACK_LIFTER_IREMITTER_H

#include "llvm/IR/IRBuilder.h"
#include "BuilderHelper.h"
#include "hackasm/Instruction.h"
#include "HackMachineState.h"
#include "BlockMap.h"
#include "hackasm/AST.h"

namespace hacklift {

    class IREmitter {
        HackMachineState &m;
        BlockMap &bmap;
        BuilderHelper &h;
        const hackasm::AST &ast;

        std::unordered_map<std::string, Value *(*)(IREmitter *)> comp_values = {
                {"0",   [](auto p) {
                    return p->h.i16(0);
                }},
                {"1",   [](auto p) {
                    return p->h.i16(1);
                }},
                {"-1",  [](auto p) {
                    return p->h.i16(-1);
                }},
                {"D",   [](auto p) {
                    Value *D = p->h.bldr.CreateLoad(p->m.D);
                    return D;
                }},
                {"A",   [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    //return A;
                    return p->m.A;
                }},
                {"!D",  [](auto p) {
                    Value *D = p->h.bldr.CreateLoad(p->m.D);
                    return p->h.op_not(D);
                }},
                {"-D",  [](auto p) {
                    Value *D = p->h.bldr.CreateLoad(p->m.D);
                    return p->h.op_neg(D);
                }},
                {"-A",  [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    //return p->h.op_neg(A);
                    return p->h.op_neg(p->m.A);
                }},
                {"D+1", [](auto p) {
                    Value *D = p->h.bldr.CreateLoad(p->m.D);
                    return p->h.op_add(D, p->h.i16(1));
                }},
                {"A+1", [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    //return p->h.op_add(A, p->h.i16(1));
                    return p->h.op_add(p->m.A, p->h.i16(1));
                }},
                {"D-1", [](auto p) {
                    Value *D = p->h.bldr.CreateLoad(p->m.D);
                    return p->h.op_sub(D, p->h.i16(1));
                }},
                {"A-1", [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    //return p->h.op_sub(A, p->h.i16(1));
                    return p->h.op_sub(p->m.A, p->h.i16(1));
                }},
                {"D+A", [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    Value *D = p->h.bldr.CreateLoad(p->m.D);
                    return p->h.op_add(D, p->m.A);
                }},
                {"D-A", [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    Value *D = p->h.bldr.CreateLoad(p->m.D);
                    return p->h.op_sub(D, p->m.A);
                }},
                {"A-D", [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    Value *D = p->h.bldr.CreateLoad(p->m.D);
                    return p->h.op_sub(p->m.A, p->m.A);
                }},

                {"D&A", [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    Value *D = p->h.bldr.CreateLoad(p->m.D);
                    return p->h.op_and(D, p->m.A);
                }},
                {"D|A", [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    Value *D = p->h.bldr.CreateLoad(p->m.D);
                    return p->h.op_or(D, p->m.A);
                }},
                {"D|M", [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    Value *D = p->h.bldr.CreateLoad(p->m.D);
                    return p->h.op_or(D, p->h.read_array(p->m.MEM, p->m.A));
                }},
                {"D&M", [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    Value *D = p->h.bldr.CreateLoad(p->m.D);
                    return p->h.op_and(D, p->h.read_array(p->m.MEM, p->m.A));
                }},
                {"M-D", [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    Value *D = p->h.bldr.CreateLoad(p->m.D);
                    return p->h.op_sub(p->h.read_array(p->m.MEM, p->m.A), D);
                }},
                {"D-M", [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    Value *D = p->h.bldr.CreateLoad(p->m.D);
                    return p->h.op_sub(D, p->h.read_array(p->m.MEM, p->m.A));
                }},
                {"D+M", [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    Value *D = p->h.bldr.CreateLoad(p->m.D);
                    return p->h.op_add(D, p->h.read_array(p->m.MEM, p->m.A));
                }},
                {"M-1", [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    return p->h.op_sub(p->h.read_array(p->m.MEM, p->m.A), p->h.i16(-1));
                }},
                {"-M",  [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    return p->h.op_neg(p->h.read_array(p->m.MEM, p->m.A));
                }},
                {"!M",  [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    return p->h.op_not(p->h.read_array(p->m.MEM, p->m.A));
                }},
                {"M",   [](auto p) {
                    //Value *A = p->h.bldr.CreateLoad(p->m.A);
                    return p->h.read_array(p->m.MEM, p->m.A);
                }}
        };

    public:
        explicit IREmitter(HackMachineState &, BlockMap &, BuilderHelper &, const hackasm::AST &);

        void operator()(const hackasm::A_Type &);

        void operator()(const hackasm::L_Type &);

        void operator()(const hackasm::C_Type &);

        void operator()(const hackasm::B_Type &);
    };


}

#endif //HACK_LIFTER_IREMITTER_H
