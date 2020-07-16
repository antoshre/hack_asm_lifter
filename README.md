# hack_asm_lifter
Hack assembly to LLVM IR lifter

Hack is a toy assembly language created for the [nand2tetris](https://www.nand2tetris.org/) course.  The simple nature of the language makes it ideal for experimenting with building LLVM IR.

This project is split into two parts:
* Assemble the Hack assembly code into an AST and resolve all symbols.
* Convert the AST into LLVM IR.

The assembler is (mostly) done, though still needs a good refactor to clean up the interfaces and make it suitable to be an independent library.
The AST converter is barely started.  LLVM's internals change at a breakneck pace and it is extremely difficult to find up-to-date information that isn't "read the source".  If nothing else this project will stand as an example of how to use LLVM's IRBuilder and built-in JIT to build IR, compile it, and run it.
