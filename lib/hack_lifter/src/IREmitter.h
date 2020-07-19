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

namespace hacklift {

    class IREmitter {
        HackMachineState &m;
        BlockMap &bmap;
        BuilderHelper &h;

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
                    return p->m.D;
                }},
                {"A",   [](auto p) {
                    return p->m.A;
                }},
                {"!D",  [](auto p) {
                    return p->h.op_not(p->m.D);
                }},
                {"-D",  [](auto p) {
                    return p->h.op_neg(p->m.D);
                }},
                {"-A",  [](auto p) {
                    return p->h.op_neg(p->m.A);
                }},
                {"D+1", [](auto p) {
                    return p->h.op_add(p->m.D, p->h.i16(1));
                }},
                {"A+1", [](auto p) {
                    return p->h.op_add(p->m.A, p->h.i16(1));
                }},
                {"D-1", [](auto p) {
                    return p->h.op_sub(p->m.D, p->h.i16(1));
                }},
                {"A-1", [](auto p) {
                    return p->h.op_sub(p->m.A, p->h.i16(1));
                }},
                {"D+A", [](auto p) {
                    return p->h.op_add(p->m.D, p->m.A);
                }},
                {"D-A", [](auto p) {
                    return p->h.op_sub(p->m.D, p->m.A);
                }},
                {"A-D", [](auto p) {
                    return p->h.op_sub(p->m.A, p->m.D);
                }},
                {"D&A", [](auto p) {
                    return p->h.op_and(p->m.D, p->m.A);
                }},
                {"D|A", [](auto p) {
                    return p->h.op_or(p->m.D, p->m.A);
                }},
                {"D|M", [](auto p) {
                    return p->h.op_or(p->m.D, p->h.read_array(p->m.M, p->m.A));
                }},
                {"D&M", [](auto p) {
                    return p->h.op_and(p->m.D, p->h.read_array(p->m.M, p->m.A));
                }},
                {"M-D", [](auto p) {
                    return p->h.op_sub(p->h.read_array(p->m.M, p->m.A), p->m.D);
                }},
                {"D-M", [](auto p) {
                    return p->h.op_sub(p->m.D, p->h.read_array(p->m.M, p->m.A));
                }},
                {"D+M", [](auto p) {
                    return p->h.op_add(p->m.D, p->h.read_array(p->m.M, p->m.A));
                }},
                {"M-1", [](auto p) {
                    return p->h.op_sub(p->h.read_array(p->m.M, p->m.A), p->h.i16(-1));
                }},
                {"-M",  [](auto p) {
                    return p->h.op_neg(p->h.read_array(p->m.M, p->m.A));
                }},
                {"!M",  [](auto p) {
                    return p->h.op_not(p->h.read_array(p->m.M, p->m.A));
                }},
                {"M",   [](auto p) {
                    return p->h.read_array(p->m.M, p->m.A);
                }}
        };

    public:
        explicit IREmitter(HackMachineState &, BlockMap &, BuilderHelper &);

        void operator()(const hackasm::A_Type &);

        void operator()(const hackasm::L_Type &);

        void operator()(const hackasm::C_Type &);
    };


}

#endif //HACK_LIFTER_IREMITTER_H
