# hack_asm_lifter
### LLVM IR Static Recompiler for Hack Assembly

Hack is a toy assembly language created for the [nand2tetris](https://www.nand2tetris.org/) course.  The simple nature of the language makes it ideal for experimenting with building LLVM IR.

The purpose of this project is to read in a Hack assembly file and convert it into LLVM IR.

## Installation

Clone the repo, grab the submodules with `git submodule update --init --recursive`

## Requirements 

Requires C++20.  

Requires any relatively recent version of Boost.  Will be removed as a dependency once I get std::ranges working.

Requires LLVM 10.  Currently I build and test on Linux; if you want to build on Windows then godspeed and good luck.

## Usage

See `examples/translate_asm.cpp` for a (mostly) functional demo.  `lib/src/IREmitter.cpp` is the business-end of the translation.

## Hack Instruction Support

A-Type and non-branching C-Type instructions are implemented and reasonably solid.

Branch/jump support is extremely limited.  LLVM's SSA form means some major branch analysis is required to fully support branching and I haven't cracked that nut yet.

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
```
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

