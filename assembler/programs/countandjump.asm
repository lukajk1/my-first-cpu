@5
D=A
@0
M=D // stored 5 to ram[0]

// subtract 1 from ram[0], save.
// if ram[0] gt 0, jump.
// else, continue.

// load ram[0]
@0
D=M
D=D-1
M=D // save back to M
@4
D;JGT

@99
D=A
@1
M=D
