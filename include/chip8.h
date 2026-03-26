#ifndef CHIP8_H
#define CHIP8_H

#include <chrono>
#include <cstdint>
#include <random>
#include <string>

class CHIP8 {
public:
    // predefined constants
    static constexpr uint8_t DISP_WIDTH { 64 };
    static constexpr uint8_t DISP_HEIGHT { 32 };

    CHIP8();

    // Load a binary instruction file into chip's ROM
    void loadROM(const std::string& filename);

    // Load the font characters into memory
    void loadFont();

    // Create the function pointer table used as the central control statement for simulation
    void createPointerTable();

    // Helper functions for pointer table opcode decoding
    void opFunc0();
    void opFunc8();
    void opFuncE();
    void opFuncF();

    // Simulate one full processor cycle
    void runCycle();

    // Debugging functions to print memory/registers
    void printROM(uint16_t start, uint16_t count) const;
    void printRegs() const;

    // Getters for fields of opcode
    uint16_t opGetAddr() const;
    uint8_t opGetN() const;
    uint8_t opGetX() const;
    uint8_t opGetY() const;
    uint8_t opGetKK() const;

    // Calculate memory address corresponding to given display coordinates
    uint16_t getDisplayAddr(uint8_t x, uint8_t y) const;

    // Finds which key has been pressed, or returns out-of-range key if none found
    uint8_t keyPressed() const;

    // 
    // INSTRUCTION SET
    // 

    void OP_CLS();
    void OP_RET();
    void OP_JP();
    void OP_CALL();
    void OP_SEvb();
    void OP_SNEvb();
    void OP_SEvv();
    void OP_LDvb();
    void OP_ADDvb();
    void OP_LDvv();
    void OP_OR();
    void OP_AND();
    void OP_XOR();
    void OP_ADDvv();
    void OP_SUB();
    void OP_SHR();
    void OP_SUBN();
    void OP_SHL();
    void OP_SNEvv();
    void OP_LDi();
    void OP_JPv();
    void OP_RND();
    void OP_DRW();
    void OP_SKP();
    void OP_SKNP();
    void OP_LDvd();
    void OP_LDvk();
    void OP_LDdv();
    void OP_LDsv();
    void OP_ADDiv();
    void OP_LDfv();
    void OP_LDbv();
    void OP_LDiv();
    void OP_LDvi();

private:
    // predefined constants
    static constexpr uint16_t ROM_START { 0x200 };
    static constexpr uint16_t FONT_START { 0x050 };
    static constexpr uint8_t FONT_HEIGHT { 5 };
    static constexpr uint16_t FONT_SIZE { 80 };

    static constexpr uint8_t KEYMAP_SIZE { 16 };

    // font characters
    static const uint8_t FONT[FONT_SIZE];

    // pointer tables for opcode decoding
    using opFunc = void (CHIP8::*)();
    static opFunc map[16];
    static opFunc map0[15];
    static opFunc mapE[15];
    static opFunc map8[15];
    static opFunc mapF[0x66];

    // memory structure
    // 0x000 - 0x04F: unused
    // 0x050 - 0x0A0: built-in chars
    // 0x0A1 - 0x1FF: unused
    // 0x200 - 0xFFF: ROM instructions + free space

    // architecture
    uint8_t regs[16] {};      // 16 registers, V0 to VF
    uint8_t mem[4096] {};     // 4kB of memory
    uint16_t indexReg {};     // for indexing into memory
    uint16_t pc;              // program counter
    uint16_t stack[16] {};    // 16-cell stack
    uint8_t sp {};            // stack pointer

    uint16_t opcode {};

    // timing and I/O
    uint8_t delayTimer {};                          //
    uint8_t soundTimer {};                          //
    bool keyMap[KEYMAP_SIZE] {};                    // input key mappings
    uint32_t display[DISP_WIDTH * DISP_HEIGHT] {};  // 64 x 32 screen

    // RNG engine
    std::default_random_engine rng;
    std::uniform_int_distribution<uint8_t> rngByte;
};

#endif
