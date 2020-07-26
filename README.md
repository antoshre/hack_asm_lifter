# hack_asm_lifter
### LLVM IR Static Recompiler for Hack Assembly

Hack is a toy assembly language created for the [nand2tetris](https://www.nand2tetris.org/) course.  The simple nature of the language makes it ideal for experimenting with building LLVM IR.

The purpose of this project is to read in a Hack assembly file and convert it into LLVM IR.

This project relies on my [hackasm](https://github.com/antoshre/hack_asm_assembler) assembler.
 
### Todo:
* Handle keyboard input
    - Stubs inserted for handling keyboard. Still need a cross-platform way to handle non-blocking terminal I/O.
* Handle emulated screen output
    - Screen stub inserted, not yet hooked up.

Because writing to the screen memory is generally done with computed jumps and I can't support that statically,
screen output likely won't be instrumented at the instruction level.  I'll spin up a thread and copy out the contents every few milliseconds
and live with that for the foreseeable future.

## Installation

Clone the repo, grab the submodules with `git submodule update --init --recursive`

## Requirements 

Requires C++20 for std::ranges and [CTRE](https://github.com/hanickadot/compile-time-regular-expressions) used in [hackasm](https://github.com/antoshre/hack_asm_assembler) to assemble the AST.

Requires LLVM 10.  Currently I build and test on Linux; if you want to build on Windows then godspeed and good luck.

## Usage

See
[`examples/translate_asm.cpp`](https://github.com/antoshre/hack_asm_lifter/blob/master/examples/translate_asm.cpp):
```cpp
/*
 * Open the assembly file for reading.
 */
std::ifstream file(argv[1], std::ios::in);
if (!file) {
    std::cerr << "Could not open file: " << argv[1] << std::endl;
    return -2;
}
/*
 * See github.com/antoshre/hack_asm_assembler for details.
 */
hackasm::AST ast(file);

auto ctx = std::make_unique<LLVMContext>();
auto mod = std::make_unique<Module>("module", *ctx);

/*
 * Parse the AST into an LLVM module named "module", and create a void(*)(int16_t*) function named "f".
 * This is where all the IR generation happens.
 */
hacklift::parse_asm_file(*mod, ast);

std::cout << "Unoptimized: (" << mod->getFunction("f")->getInstructionCount() << " insts)" << std::endl;
hacklift::print_module(*mod);

/*
 * Use llvm::verifyModule to check the structure of the generated IR.
 * This will throw an exception if a problem is found.
 */
hacklift::verify_module(*mod);

/*
 * Optional.  Runs the equivalent of "-O3" optimization on the generated IR.
 */
hacklift::optimize_module(*mod);

std::cout << "\nOptimized: (" << mod->getFunction("f")->getInstructionCount() << " insts)" << std::endl;
hacklift::print_module(*mod);

/*
 * The generated IR assumes an int16_t* is passed in as a parameter,
 * this represents the "memory" for the transpiled program to work on.
 * Until keyboard and screen handling are in this is the only way to get data into and out of the system.
 * Soon (TM) it will also return the contents of the A and D registers.
 */
std::array<int16_t, 32768> mem{0};
mem[0] = 50;
mem[1] = 85;

/*
 * hacklift::run_void_func will print the first 16 elements of the memory array before and after the IR is run.
 * Eg:
 * Memory before run:
 * 0032 0055 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 
 * Memory after run:
 * 0032 0055 0087 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
 *  
 *  WARNING: The structure of the code to run the module require it to be moved so `mod` should be considered dead!
 */
hacklift::run_void_func(std::move(mod), std::move(ctx), "f", mem);

return 0;
```

[`lib/hack_lifter/src/IREmitter.cpp`](https://github.com/antoshre/hack_asm_lifter/blob/master/lib/hack_lifter/src/IREmitter.cpp)
and
[`lib/hack_lifter/src/BuilderHelper.cpp`](https://github.com/antoshre/hack_asm_lifter/blob/master/lib/hack_lifter/src/BuilderHelper.cpp)
are the business-end of the translation.

## Example Output

Memory is modeled as an int16[] passed as a parameter to allow data transfer into and out of the function.

Add.asm:
```
//Add R0 to R1 and store in R2
@R0 //A=0
D=M //D=M[0]
@R1 //A=1
A=M //A=M[1]
D=D+A //D=M[0]+M[1]
@R2 //A=2
M=D //M[2]=D
```
LLVM IR:
```cpp
/tmp/tmp.td8vzcShmB/cmake-build-debug/examples/translate_asm ../../examples/Add.asm
Unoptimized: (8 insts)
; ModuleID = 'module'
source_filename = "module"

define void @f(i16* %MEM) {
entry:
  %0 = getelementptr i16, i16* %MEM, i16 0
  %1 = load i16, i16* %0
  %2 = getelementptr i16, i16* %MEM, i16 1
  %3 = load i16, i16* %2
  %4 = add i16 %1, %3
  %5 = getelementptr i16, i16* %MEM, i16 2
  store i16 %4, i16* %5
  ret void
}

Memory before run:
0032 0055 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 
Memory after run:
0032 0055 0087 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 
```

## Hack Instruction Support

All instruction types are implemented, though testing needs to be implemented.  The framework is in but the implementation is tricky.  
Testing for functional equivalence on a per-instruction basis is largely impossible, the vast majority of the issues I've
encountered are from multiple instruction interactions.


~~Branch/jump support is extremely limited.  LLVM's SSA form means some major branch analysis is required to fully support branching and I haven't cracked that nut yet.~~

Branching support is in, though I need more programs and a better way to test for regressions.  SSA limitations worked around by
running all access to 'D' register through memory, which is apparently a common trick used by LLVM itself for this same reason.

The upshot is the unoptimized IR will have a ton of loads and stores that are blown away by optimization.
