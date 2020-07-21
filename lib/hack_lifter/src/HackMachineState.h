//
// Created by rob on 7/18/2020.
//

#ifndef HACK_LIFTER_HACKMACHINESTATE_H
#define HACK_LIFTER_HACKMACHINESTATE_H

#include "llvm/IR/Value.h"

namespace hacklift {
    struct HackMachineState {
        Value *A, *D, *M, *MEM;
    };
}

#endif //HACK_LIFTER_HACKMACHINESTATE_H
