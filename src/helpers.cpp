#include "../include/helpers.h"

std::string toBinary(Word w) {
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

std::string toHex(Word w) {
    std::string result = "0x";
    const char* hexChars = "0123456789ABCDEF";

    for (int i = 3; i >= 0; i--) {
        // Shift by 12, 8, 4, then 0 bits to isolate each nibble
        // Mask with 0xF (1111 in binary) to get the value 0-15
        int nibble = (w >> (i * 4)) & 0xF;
        result += hexChars[nibble];
    }
    return result;
}