@5
// d=a+1;jeq
// comp = a+1, dest = d, jump = jeq
D = A // copy a into d
// comp = a, d = dest, 000 = jump
// 111 + comp(7) + dest(3) + jump(3)

// D = A -> 0010010
@0
M = D