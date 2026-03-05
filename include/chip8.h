#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <string>

class CHIP8 {
public:
    CHIP8();

    // Load a binary instruction file into chip's ROM
    void loadROM(const std::string& filename);

    // Load the font characters into memory
    void loadFont();

    // debugging
    void printROM(uint16_t start, uint16_t count) const;

private:
    // predefined constants
    static const uint16_t ROM_START { 0x200 };
    static const uint16_t FONT_START { 0x050 };
    static const size_t FONT_SIZE { 80 };

    // font characters
    static const uint8_t FONT[FONT_SIZE];

    // memory structure
    // 0x000 - 0x04F: unused
    // 0x050 - 0x0A0: built-in chars
    // 0x0A1 - 0x1FF: unused
    // 0x200 - 0xFFF: ROM instructions + free space

    // architecture
    std::uint8_t regs[16] {};      // 16 registers, V0 to VF
    std::uint8_t mem[4096] {};     // 4kB of memory
    std::uint16_t indexReg {};     // for indexing into memory
    std::uint16_t pc;              // program counter
    std::uint16_t stack[16] {};    // 16-cell stack
    std::uint8_t sp {};            // stack pointer

    std::uint16_t opcode {};

    // timing and I/O
    std::uint8_t delayTimer {};        //
    std::uint8_t soundTimer {};        //
    bool keyMap[16] {};                // input key mappings
    std::uint32_t display[64 * 32] {}; // 64x32 screen
};

#endif
