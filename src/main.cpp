#include <iomanip>
#include <iostream>
#include <thread> // For sleep_for
#include <chrono> // For milliseconds

#include "../include/gates.h"
#include "../include/helpers.h"


int main() {
    Word instruction = 0b1;

    Counter pc;
    bool load = false;
    Word jumpAddress = 0;

    // Run the simulation for 20 cycles
    for (int i = 0; i < 20; ++i) {
        // Update the counter state
        pc.tick(load, jumpAddress);

        // Display the current value (using your printHex or printBinary helpers)
        std::cout << "Cycle " << i << " | PC: " << pc.reg.out << std::endl;

        // Delay for 500 milliseconds (half a second)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    //Word x = 0x1234; // 0001 0010 0011 0100
    //Word y = 0xABCD; // 1010 1011 1100 1101

    //y = 0x1111;
    //x = 0xBEEF;

    ////std::cout << "x: " << x << std::endl;
    ////std::cout << "y: " << y << std::endl;

    ////ALU alu;
    ////bool u, op1, op0, zx, sw;
    ////u = op1 = op0 = zx = sw = false;

    //////u = op0 =op1 = true;
    //////u = op0  = true;
    //////op1 = op0  = true;
    //////op0 = true;
    ////u =op1= true;

    ////alu.compute(x, y, u, op1, op0, zx, sw);
    ////
    ////std::cout << "binary: " << toBinary(alu.out) << std::endl;
    ////std::cout << "decimal: " << alu.out << std::endl;

    //Reg16 myReg;
    //myReg.tick(true, x);
    //std::cout << "reg contents: " << myReg.out << std::endl;

    //Word ramAddress = 0x0001;
    //Word beef = 0xBEEF;

    //AddressableMemory am;
    //am.tick(true, false, false, ramAddress);
    //std::cout << "ram address: " << am.A << std::endl;

    //am.tick(false, false, true, beef);
    //std::cout << "value at " << toHex(am.A) << ": " << toHex(am.A_Star) << std::endl;


    return 0;
}
