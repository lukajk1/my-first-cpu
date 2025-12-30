#include <iomanip>
#include <iostream>
#include "../include/gates.h"
#include "../include/helpers.h"

struct LogicUnit {
    Word out;

    Or16  or16;
    And16 and16;
    Inv16 inv16;
    Xor16 xor16;
    Sel16 sel0, sel1, sel2;

    void compute(Word x, Word y, bool op0, bool op1) {
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
        sel2.compute(op1, sel0.out, sel1.out);

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

    void compute(Word x, Word y, bool op0, bool op1) {
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

int main() {
    LogicUnit lu;
    Word x = 0x1234; // 0001 0010 0011 0100
    Word y = 0xABCD; // 1010 1011 1100 1101

    ArithmeticUnit au;

    std::cout << "x: " << x << " y: " << y << std::endl;

    au.compute(x, y, 0, 0);
    std::cout << "x + y: " << au.out << std::endl;

    au.compute(x, y, 1, 0);
    std::cout << "x - y: " << au.out << std::endl;

    return 0;
}
