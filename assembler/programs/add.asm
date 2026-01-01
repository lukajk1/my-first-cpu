// Simple addition program
// Computes 5 + 3 and stores in RAM[0]

@5      // A = 5
D=A     // D = 5

@3      // A = 3
D=D+A   // D = 5 + 3 = 8

@0      // A = 0
M=D     // RAM[0] = 8

@10
0;JMP   // Infinite loop
