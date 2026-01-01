#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main() {
    // Open the .asm file
    std::ifstream asmFile("programs/add.asm");

    if (!asmFile.is_open()) {
        std::cerr << "Error: Could not open programs/add.asm" << std::endl;
        return 1;
    }

    std::cout << "=== Assembly File Contents ===" << std::endl;
    std::cout << std::endl;

    std::string line;
    int lineNum = 1;

    // Read file line by line
    while (std::getline(asmFile, line)) {
        std::cout << lineNum << ": " << line << std::endl;
        lineNum++;
    }

    asmFile.close();

    std::cout << std::endl;
    std::cout << "File loaded successfully!" << std::endl;

    return 0;
}
