#include <iomanip>
#include <iostream>
#include "../include/gates.h"
#include "../include/helpers.h"

int main() {
    Word program[256] = {0};

    // a=67 @67
    program[0] = 0b000'00'00001'000'011;  
    // a* = a
    program[1] = 0b111'00'10010'001'000;
    //
    //// C-instruction: D=A
    //program[2] = 0b111'00'10010'010'000;

    //program[3] = 0b111'00'10010'010'000;

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
