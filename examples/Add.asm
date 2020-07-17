//Add R0 to R1 and store in R2
@R0 //A=0
D=M //D=M[0]
@R1 //A=1
A=M //A=M[1]
D=D+A //D=M[0]+M[1]
@R2 //A=2
M=D //M[2]=D