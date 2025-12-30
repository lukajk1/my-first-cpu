#include <iomanip>
#include <iostream>
#include "../include/gates.h"
#include "../include/helpers.h"


int main() {
    Word instruction = 0b1;

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
