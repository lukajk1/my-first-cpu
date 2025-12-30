#include "../include/helpers.h"

std::string printBinary(Word w) {
    std::string result;
    for (int i = 15; i >= 0; i--) {
        // Check if the i-th bit is set
        result += ((w >> i) & 1) ? '1' : '0';

        // Add a space after every 4 bits (except the last)
        if (i % 4 == 0 && i != 0) {
            result += ' ';
        }
    }
    return result;
}

