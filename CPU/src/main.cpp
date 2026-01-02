#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include "../include/gates.h"
#include "../include/helpers.h"

bool loadHackProgram(const std::string& filename, Word program[], int maxSize) {
    std::ifstream file("../assembler/programs/" + filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return false;
    }

    std::string line;
    int lineCount = 0;

    while (std::getline(file, line) && lineCount < maxSize) {
        // Remove whitespace
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

        // Skip empty lines
        if (line.empty()) continue;

        // Convert binary string to Word
        program[lineCount] = 0;
        for (char c : line) {
            program[lineCount] = (program[lineCount] << 1) | (c - '0');
        }

        lineCount++;
    }

    file.close();
    std::cout << "Loaded " << lineCount << " instructions from " << filename << std::endl;
    return true;
}

int main() {
    Word program[256] = {0};

    // Specify the .hack file to load
    std::string hackFile = "copy5intoD.hack";

    if (!loadHackProgram(hackFile, program, 256)) {
        return 1;
    }

    Computer computer(program);

    std::cout << "initial state:" << std::endl;
    std::cout << "  PC: " << computer.counter.out;
    std::cout << " | A: " << computer.memory.A;
    std::cout << " | D: " << computer.memory.D;
    std::cout << " | RAM[0]: " << computer.memory.ram.registers[0].out;
    std::cout << std::endl;

    // Run for 10 cycles
    for (int cycle = 0; cycle < 10; cycle++) {
        computer.tick();

        std::cout << "state post-line " << cycle << ":" << std::endl;
        std::cout << "  PC: " << computer.counter.out;
        std::cout << " | A: " << computer.memory.A;
        std::cout << " | D: " << computer.memory.D;
        std::cout << " | RAM[0]: " << computer.memory.ram.registers[0].out;
        std::cout << std::endl;

    }

    std::cout << std::endl;
    std::cout << "Final State:" << std::endl;
    std::cout << "  A register: " << computer.memory.A << std::endl;
    std::cout << "  D register: " << computer.memory.D << std::endl;    
    std::cout << " | RAM[0]: " << computer.memory.ram.registers[0].out;

    std::cout << std::endl;

    return 0;
}
