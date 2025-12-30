#ifndef GATES_H
#define GATES_H

#include "aliases.h"

struct Or16 {
    Word out = 0;
    void compute(Word a, Word b) { out = a | b; }
};

struct Inv16 {
    Word out = 0;
    void compute(Word a) { out = ~a; }
};

struct And16 {
    Word out = 0;
    void compute(Word a, Word b) { out = a & b; }
};

struct Xor16 {
    Word out = 0;
    void compute(Word a, Word b) { out = a ^ b; }
};

struct Add16 {
    Word out = 0;
    void compute(Word a, Word b) { out = a + b; }
};

struct Sub16 {
    Word out = 0;
    void compute(Word a, Word b) { out = a - b; }
};

struct Sel16 {
    Word out = 0;
    void compute(bool sel, Word d1, Word d0) {
        out = sel ? d1 : d0;
    }
};

#endif // GATES_H
