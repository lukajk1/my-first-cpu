@5
D=A
@0
M=D // stored 5 to ram[0]

@5
D=A
@1
M=D // stored 5 to ram[1]
// begin loop
// ram[1] = i
// compare ram[1] to ram[0]

@1 // prepare to retrieve ram[1]
M=M-1 // ram[1] += 1, address of M is always value A, ram[1] = i
D=M
// write this to ram[*A]
A=D
M=A // ram[i+1] = i (hopefully)

@1



