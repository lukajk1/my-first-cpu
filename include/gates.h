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

struct LogicUnit {
    Word out;

    Or16  or16;
    And16 and16;
    Inv16 inv16;
    Xor16 xor16;
    Sel16 sel0, sel1, sel2;

    void compute(Word x, Word y, bool op1, bool op0) {
        // Step 1: Logic layer
        or16.compute(x, y);
        and16.compute(x, y);
        inv16.compute(x);
        xor16.compute(x, y);

        // Step 2: Selection layer (Muxing)
        // sel0 chooses between OR and AND
        sel0.compute(op0, or16.out, and16.out);

        // sel1 chooses between NOT and XOR
        sel1.compute(op0, inv16.out, xor16.out);

        // sel2 chooses between the results of sel0 and sel1
        sel2.compute(op1, sel1.out, sel0.out);

        out = sel2.out;
    }
};

struct ArithmeticUnit {
    Word out;

    Add16 add16_0;
    Sub16 sub16_0;
    Add16 add16_1;
    Sub16 sub16_1;
    Sel16 sel0, sel1, sel2;

    void compute(Word x, Word y, bool op1, bool op0) {
        add16_0.compute(x, y);
        sub16_0.compute(x, y);
        add16_1.compute(x, 1);
        sub16_1.compute(x, 1);

        sel0.compute(op1, sub16_0.out, add16_0.out);
        sel1.compute(op1, sub16_1.out, add16_1.out);
        sel2.compute(op0, sel1.out, sel0.out);

        out = sel2.out;
    }
};

struct ALU {
    Word out;

    ArithmeticUnit au;
    LogicUnit lu;
    Sel16 sel0, sel1, sel2, sel3;

    void compute(Word x, Word y, bool u, bool op1, bool op0, bool zx, bool sw) {
        sel0.compute(sw, y, x);
        sel1.compute(sw, x, y);
        sel2.compute(zx, 0, sel0.out);

        lu.compute(sel2.out, sel1.out, op1, op0);
        au.compute(sel2.out, sel1.out, op1, op0);
        sel3.compute(u, au.out, lu.out);

        out = sel3.out;
    }

};

struct Condition {
    bool out;

    void compute(bool lt, bool eq, bool gt, Word x) {
        bool is_neg = (x >> 15) & 1;
        bool is_zero = (x == 0);
        bool is_pos = !is_neg && !is_zero;

        out = (lt && is_neg) || (eq && is_zero) || (gt && is_pos);
    }
};


struct SRLatch {
    bool out;
    void compute(bool s, bool r) {
        if (s && r) {
            // do nothing
        }
        else if (s) {
            out = true;
        }
        else if (r) {
            out = false;
        }
    }
};

struct DLatch {
    bool d;
    void compute(bool st, bool d) {
        if (st) {
            this->d = d;
        }
    }
};

struct Reg16 {
    Word out;
    void tick(bool st, Word x) {
        if (st) {
            out = x;
        }
    }
};

struct Counter {
    Reg16 reg;
    Sel16 sel;
    Add16 inc;

    Word out;

    Counter() {
        reg.out = 0;
    }

    void tick(bool st, Word x) {
        inc.compute(reg.out, 1);

        sel.compute(st, x, inc.out);
        reg.tick(true, sel.out);

        out = reg.out;
    }
};

struct RegisterFile {
    Reg16 registers[256];

    Word out;

    void tick(Word address, Word x, bool st) {
        // restrict to 0-255
        if (address >= 256) {
            std::cout << "address was out of bounds" << std::endl;
            return;
        }

        registers[address].tick(st, x);

        out = registers[address].out;
    }
};

struct ReadOnlyMemory {
    Word registers[256];
    Word out;

    ReadOnlyMemory(const Word program[256]) {
        for (int i = 0; i < 256; i++) {
            registers[i] = program[i];
        }
        out = 0;
    }

    void compute(Word address) {
        // Use bitwise masking to "force" the address into the 0-255 range
        // This is how hardware actually works (ignoring higher bits)
        Word effectiveAddress = address & 0xFF;

        // Access the value directly from the array
        out = registers[effectiveAddress];
    }
};

struct AddressableMemory {
    Reg16 reg_a, reg_d;
    RegisterFile ram;
    Word A, D, A_Star;

    void tick(bool a, bool d, bool a_star, Word x) {
        reg_a.tick(a, x);
        A = reg_a.out;

        reg_d.tick(d, x);
        D = reg_d.out;

        ram.tick(a_star, x, reg_a.out);
        A_Star = ram.out;
    }
};

struct ALUInstruction {
    ALU alu;
    Sel16 sel16;
    Condition cond;
    
    // outputs 
    Word R;
    bool a, d, a_star, j;

    void compute(Word I, Word A, Word D, Word A_Star) {
        bool u = (I >> 10) & 1;
        bool op1 = (I >> 9) & 1;
        bool op0 = (I >> 8) & 1;
        bool zx = (I >> 7) & 1;
        bool sw = (I >> 6) & 1;
        a = (I >> 5) & 1;
        d = (I >> 4) & 1;
        a_star = (I >> 3) & 1;
        bool lt = (I >> 2) & 1;
        bool eq = (I >> 1) & 1;
        bool gt = I & 1;

        bool w = (I >> 12) & 1;
        sel16.compute(w, A_Star, A);

        alu.compute(D, sel16.out, u, op1, op0, zx, sw);
        R = alu.out;
        cond.compute(lt, eq, gt, alu.out);
        j = cond.out;

    }
};

struct ControlSelector {
    Sel16 sel0, sel1, sel2, sel3, sel4;

    Word R;
    bool a, d, a_star, j;
    void compute(bool s, Word R_0, bool a_0, bool d_0, bool a_star_0, bool j_0, Word R_1, bool a_1, bool d_1, bool a_star_1, bool j_1) {
        sel0.compute(s, R_0, R_1);
        R = sel0.out;

        a = s ? a_0 : a_1;
        d = s ? d_0 : d_1;
        a_star = s ? a_star_0 : a_star_1;
        j = s ? j_0 : j_1;
    }
};

struct ControlUnit {
    ALUInstruction alui;
    ControlSelector controlSel;

    // output
    Word R;
    bool a, d, a_star, j;

    void compute(Word I, Word A, Word D, Word A_Star) {
        bool s = (I >> 15) & 1;

        alui.compute(I, A, D, A_Star);
        controlSel.compute(s, alui.R, alui.a, alui.d, alui.a_star, alui.j, I, true, false, false, false);

        R = controlSel.R;
        a = controlSel.a;
        d = controlSel.d;
        a_star = controlSel.a_star;
        j = controlSel.j;
    }
};

struct Computer {
    Counter counter;
    ControlUnit controlUnit;
    AddressableMemory memory;
    ReadOnlyMemory rom;

    bool st;
    Computer() {
        st = 0;
    }

    void tick() {
        counter.tick(st, );
        rom.compute(counter.out);
        controlUnit.compute(rom.out, )
    }
};

#endif // GATES_H
