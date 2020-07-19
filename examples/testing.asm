   @R0
   D=M              // D = first number
   @R1
   D=D-M            // 85-50 = 30 > 0
   @DGREATER
   D;JGT
//DLESSER
	@R1
	D=M
	@R2
	M=D
	@EXIT
	0;JMP
   
   
(DGREATER)
	@R0
	D=M
	@R2
	M=D
	@EXIT
	0;JMP
	
(EXIT)
@EXIT
0;JMP