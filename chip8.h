#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>

class chip8 {

private:
    // registers
    // memory
    std::uint16_t indexReg; // for indexing into memory
    std::uint16_t pc; // program counter
    // stack (16 cells)
    // stack pointer
};

#endif
