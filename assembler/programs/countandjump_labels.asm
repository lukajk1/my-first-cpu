// Countdown with labels and predefined symbols
// Counts down R0 from 5 to 0 using labels

@5
D=A
@R0
M=D         // R0 = 5

(LOOP)
@R0
D=M         // D = R0
D=D-1       // D = D - 1
M=D         // R0 = D

@LOOP
D;JGT       // If D > 0, jump to LOOP

// Store completion flag in R1
@99
D=A
@R1
M=D         // R1 = 99 (completion flag)

(END)
@END
0;JMP       // Infinite loop - trap here
