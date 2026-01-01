#include <iomanip>
#include <iostream>
#include "../include/gates.h"
#include "../include/helpers.h"

int main() {
    Word program[256] = {0};

    // A-instruction: @5
    program[0] = 0b0000000000000101;  

    // C-instruction: D=A
    program[1] = 0b111'01'10010'010'000;  

    // A-instruction: @3
    program[2] = 0b000'00'00000'000'011;  

    // c inst: d = d+a
    program[3] = 0b111'00'10000'010'000;  

    // a=67 @67
    program[4] = 0b000'00'00001'000'011;  

    // a* = a
    program[5] = 0b111'00'10010'001'000;  


    // a* = d (ram[a] = d)
    //program[5] = 0b111'10'00111'001'000;  
    // 
    //@0
    //program[7] = 0b000'00'00000'000'000;  


    Computer computer(program);

    std::cout << "initial state:" << std::endl;
    std::cout << "  PC: " << computer.counter.out;
    std::cout << " | A: " << computer.memory.A;
    std::cout << " | D: " << computer.memory.D;
    std::cout << " | RAM[67]: " << computer.memory.ram.registers[67].out;
    std::cout << std::endl;

    // Run for 10 cycles
    for (int cycle = 0; cycle < 10; cycle++) {
        computer.tick();

        std::cout << "state post-line " << cycle << ":" << std::endl;
        std::cout << "  PC: " << computer.counter.out;
        std::cout << " | A: " << computer.memory.A;
        std::cout << " | D: " << computer.memory.D;
        std::cout << " | RAM[67]: " << computer.memory.ram.registers[67].out;
        std::cout << std::endl;

    }

    std::cout << std::endl;
    std::cout << "Final State:" << std::endl;
    std::cout << "  A register: " << computer.memory.A << std::endl;
    std::cout << "  D register: " << computer.memory.D << std::endl;    
    std::cout << " | RAM[67]: " << computer.memory.ram.registers[67].out;

    std::cout << std::endl;

    return 0;
}
