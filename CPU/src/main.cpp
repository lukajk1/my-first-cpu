#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include "../include/gates.h"
#include "../include/helpers.h"

int loadHackProgram(const std::string& filename, Word program[], int maxSize);
void printState(const Computer& computer, const std::string& label);

std::string hackFile = "loadintoram.hack";

int main() {
    Word program[256] = { 0 };

    int instructionCount = loadHackProgram(hackFile, program, 256);
    if (instructionCount < 0) {
        return 1;
    }

    Computer computer(program);

    std::cout << std::endl;
    printState(computer, "initial state:");

    // Run for as many cycles as there are instructions
    for (int cycle = 0; cycle < instructionCount; cycle++) {
        computer.tick();
        printState(computer, "state post-line " + std::to_string(cycle) + ":");
    }

    std::cout << std::endl;
    printState(computer, "Final State:");

    return 0;
}

int loadHackProgram(const std::string& filename, Word program[], int maxSize) {
    std::ifstream file("../assembler/programs/" + filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return -1;
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
    return lineCount;
}

void printState(const Computer& computer, const std::string& label) {
    std::cout << label << std::endl;
    std::cout << "  PC: " << computer.counter.out;
    std::cout << " | A: " << computer.memory.A;
    std::cout << " | D: " << computer.memory.D;
    std::cout << " | RAM[0]: " << computer.memory.ram.registers[0].out;
    std::cout << std::endl;
}

