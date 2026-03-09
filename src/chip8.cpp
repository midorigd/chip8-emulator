#include "chip8.h"

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

CHIP8::CHIP8() : rng(std::chrono::system_clock::now().time_since_epoch().count()) {
    pc = ROM_START;

    rngByte = std::uniform_int_distribution<uint8_t>(0, 255U);
}

void CHIP8::loadROM(const std::string& filename) {
    std::ifstream romFile(filename, std::ios::binary);

    if (!romFile) {
        std::cerr << "ROM file " << filename << " not opened\n";
        exit(1);
    }

    uint16_t i { 0 };

    // read in file in chunks of 1 byte
    while (romFile) {
        romFile.read(reinterpret_cast<char*>(mem + ROM_START + i), 1);
        ++i;
    }
}

void CHIP8::loadFont() {
    for (uint16_t i = 0; i < FONT_SIZE; ++i) {
        mem[FONT_START + i] = FONT[i];
    }
}

uint16_t CHIP8::opGetAddr() const {
    return opcode & 0x0FFF;
}

uint8_t CHIP8::opGetN() const {
    return opcode & 0x000F;
}

uint8_t CHIP8::opGetX() const {
    return (opcode >> 8) & 0x000F;
}

uint8_t CHIP8::opGetY() const {
    return (opcode >> 4) & 0x000F;
}

uint8_t CHIP8::opGetKK() const {
    return opcode & 0x00FF;
}

void CHIP8::OP_CLS() {
    memset(display, 0, sizeof(display));
}

void CHIP8::OP_RET() {
    --sp;
    pc = stack[sp];
}

void CHIP8::OP_JP() {
    pc = opGetAddr();
}

void CHIP8::OP_CALL() {
    stack[sp] = pc;
    ++sp;
    pc = opGetAddr();
}

void CHIP8::OP_SEvb() {
    if (regs[opGetX()] == opGetKK()) {
        pc += 2;
    }
}

void CHIP8::OP_SNEvb() {
    if (regs[opGetX()] != opGetKK()) {
        pc += 2;
    }
}

void CHIP8::OP_SEvv() {
    if (regs[opGetX()] == regs[opGetY()]) {
        pc += 2;
    }
}

void CHIP8::OP_LDvb() {
    regs[opGetX()] = opGetKK();
}

void CHIP8::OP_ADDvb() {
    regs[opGetX()] += opGetKK();
}

void CHIP8::OP_LDvv() {
    regs[opGetX()] = regs[opGetY()];
}

void CHIP8::OP_OR() {
    regs[opGetX()] |= regs[opGetY()];
}

void CHIP8::OP_AND() {
    regs[opGetX()] &= regs[opGetY()];
}

void CHIP8::OP_XOR() {
    regs[opGetX()] ^= regs[opGetY()];
}

void CHIP8::OP_ADDvv() {
    uint8_t x { opGetX() };
    uint8_t y { opGetY() };

    uint8_t result { regs[x] + regs[y] };

    // set overflow flag
    regs[0xF] = (result < regs[x]);

    regs[x] = result;
}

void CHIP8::OP_SUB() {
    uint8_t x { opGetX() };
    uint8_t y { opGetY() };

    // set NOT borrow flag
    regs[0xF] = (regs[x] > regs[y]);

    regs[x] -= regs[y];
}

void CHIP8::OP_SHR() {
    uint8_t x { opGetX() };

    // set VF to LSB of Vx
    regs[0xF] = regs[x] & 1;

    regs[x] >>= 1;
}

void CHIP8::OP_SUBN() {
    uint8_t x { opGetX() };
    uint8_t y { opGetY() };

    // set NOT borrow flag
    regs[0xF] = (regs[y] > regs[x]);

    regs[x] = regs[y] - regs[x];
}

void CHIP8::OP_SHL() {
    uint8_t x { opGetX() };

    // set VF to MSB of Vx
    regs[0xF] = regs[x] & 0x80;

    regs[x] <<= 1;
}

void CHIP8::OP_SNEvv() {
    if (regs[opGetX()] != regs[opGetY()]) {
        pc += 2;
    }
}

void CHIP8::OP_LDi() {
    indexReg = opGetAddr();
}

void CHIP8::OP_JPv() {
    pc = opGetAddr() + regs[0];
}

void CHIP8::OP_RND() {
    regs[opGetX()] = rngByte(rng) & opGetKK();
}

// FIX THIS

// void CHIP8::OP_DRW() {
//     uint8_t spriteSize { opGetN() };
//     uint16_t addr { getDisplayAddr(regs[opGetX()], regs[opGetY()]) };

//     uint8_t overwrite { 0 };

//     for (size_t i = 0; i < spriteSize; ++i) {
//         uint8_t pixel { mem[regs[indexReg] + i] };

//         if (display[addr] && pixel) {
//             overwrite = 1;
//             display[addr] = 0;
//         }
//     }
// }

void CHIP8::OP_SKP() {
    if (keyMap[regs[opGetX()]]) {
        pc += 2;
    }
}

void CHIP8::OP_SKNP() {
    if (!keyMap[regs[opGetX()]]) {
        pc += 2;
    }
}

void CHIP8::OP_LDvd() {
    regs[opGetX()] = delayTimer;
}

void CHIP8::OP_LDvk() {
    uint8_t key;

    // wait for input
    do {
        key = keyPressed();
    } while (key == KEYMAP_SIZE);

    regs[opGetX()] = key;
}

void CHIP8::OP_LDsv() {
    regs[opGetX()] = soundTimer;
}

void CHIP8::OP_ADDiv() {
    indexReg += regs[opGetX()];
}

void CHIP8::OP_LDfv() {
    indexReg = FONT_START + regs[opGetX()] * FONT_HEIGHT;
}

void CHIP8::printROM(uint16_t start, uint16_t count) const {
    std::cout << std::hex << std::uppercase << std::setfill('0');

    for (uint16_t i = start; i < start + count; ++i) {
        std::cout << "ROM[" << i << "]: 0x"
                  << std::setw(2)
                  << static_cast<int>(mem[i]) << '\n';
    }
}

// font characters
const uint8_t CHIP8::FONT[FONT_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
