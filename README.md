# hack_asm_lifter
Hack assembly to LLVM IR lifter

Hack is a toy assembly language created for the [nand2tetris](https://www.nand2tetris.org/) course.  The simple nature of the language makes it ideal for experimenting with building LLVM IR.

This project is split into two parts:
* Assemble the Hack assembly code into an AST and resolve all symbols.
* Convert the AST into LLVM IR.

The assembler is functionally complete and well-tested.  Could use a good refactor.

A-Type and C-Type instructions are supported but not well-tested.

Major limitation: jumps are not yet implemented.

##Example
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
Memory is modeled as an int16[] passed as a parameter to allow data transfer into and out of the function.

LLVM IR:
```
define void @foo(i16* %Mem) {
entry:
  %A = alloca i16
  %D = alloca i16
  %0 = getelementptr i16, i16* %Mem, i16 0
  %1 = load i16, i16* %0
  %2 = getelementptr i16, i16* %Mem, i16 1
  %3 = load i16, i16* %2
  %4 = add i16 %1, %3
  %5 = getelementptr i16, i16* %Mem, i16 2
  store i16 %4, i16* %5
  ret void
}
```

