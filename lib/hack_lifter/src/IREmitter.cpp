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

}