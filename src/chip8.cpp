#include "chip8.h"

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

CHIP8::CHIP8() :
    rng(std::chrono::system_clock::now().time_since_epoch().count()),
    rngByte(0, 255) {

    pc = ROM_START;
    loadFont();
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

// void CHIP8::createPointerTable() {
//     // main pointer table
//     map[0x0] = CHIP8::OP_JP;
//     map[0x1] = CHIP8::opFunc0;
//     map[0x2] = CHIP8::OP_CALL;
//     map[0x3] = CHIP8::OP_SEvb;
//     map[0x4] = CHIP8::OP_SNEvb;
//     map[0x5] = CHIP8::OP_SEvv;
//     map[0x6] = CHIP8::OP_LDvb;
//     map[0x7] = CHIP8::OP_ADDvb;
//     map[0x8] = CHIP8::opFunc8;
//     map[0x9] = CHIP8::OP_SNEvv;
//     map[0xA] = CHIP8::OP_LDi;
//     map[0xB] = CHIP8::OP_JPv;
//     map[0xC] = CHIP8::OP_RND;
//     map[0xD] = CHIP8::OP_DRW;
//     map[0xE] = CHIP8::opFuncE;
//     map[0xF] = CHIP8::opFuncF;

//     // 0-opcode pointer table
//     map0[0x0] = CHIP8::OP_CLS;
//     map0[0XE] = CHIP8::OP_RET;

//     // 8-opcode pointer table

// }

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

uint16_t CHIP8::getDisplayAddr(uint8_t x, uint8_t y) const {
    return y * DISP_WIDTH + x;
}

uint8_t CHIP8::keyPressed() const {
    for (size_t i = 0; i < KEYMAP_SIZE; ++i) {
        if (keyMap[i]) {
            return i;
        }
    }
    return KEYMAP_SIZE;
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

    uint8_t result { static_cast<uint8_t>(regs[x] + regs[y]) };

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

void CHIP8::OP_DRW() {
    uint8_t spriteSize { opGetN() };
    uint8_t xPos { regs[opGetX()] };
    uint8_t yPos { regs[opGetY()] };

    uint8_t overwrite { 0 };

    for (size_t i = 0; i < spriteSize; ++i) {
        yPos = (yPos + i) % DISP_HEIGHT;

        // get next byte, each byte is one row of the sprite
        uint8_t byte { mem[indexReg + i] };

        // draw each pixel in the byte
        for (size_t j = 0; j < 8; ++j) {
            xPos = (xPos + j) % DISP_WIDTH;
            uint16_t addr { getDisplayAddr(xPos, yPos) };

            // get MSB from byte, this is the current pixel to draw
            uint8_t pixel = byte & 0x80;

            // pixels are XORed onto display
            if (display[addr] && pixel) {
                // check for collisions and raise overwrite flag
                overwrite = 1;
                display[addr] = 0;

            } else if (pixel) {
                display[addr] = 0xFFFFFFFF;
            }

            byte <<= 1;
        }
    }

    // overwrite flag stored into VF
    regs[0xF] = overwrite;
}

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

void CHIP8::OP_LDdv() {
    delayTimer = regs[opGetX()];
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

void CHIP8::OP_LDbv() {
    uint8_t val { regs[opGetX()] };
    int mod { 100 };

    for (int i = 0; i < 3; ++i) {
        mem[indexReg + i] = val / mod;
        mod /= 10;
    }
}

void CHIP8::OP_LDiv() {
    uint8_t x { opGetX() };

    for (uint8_t i = 0; i <= x; ++i) {
        mem[indexReg + i] = regs[i];
    }
}

void CHIP8::OP_LDvi() {
    uint8_t x { opGetX() };

    for (uint8_t i = 0; i <= x; ++i) {
        regs[i] = mem[indexReg + i];
    }
}

void CHIP8::printROM(uint16_t start, uint16_t count) const {
    std::cout << std::hex << std::uppercase << std::setfill('0');

    for (uint16_t i = start; i < start + count; ++i) {
        std::cout << "ROM[" << i << "]: 0x"
                  << std::setw(2)
                  << static_cast<int>(mem[i])
                  << ((i % 4 == 3) ? '\n' : '\t');
    }

    std::cout << '\n';
}

void CHIP8::printRegs() const {
    std::cout << std::hex << std::uppercase << std::setfill('0');

    for (uint8_t i = 0; i < 16; ++i) {
        std::cout << 'V' << static_cast<int>(i) << ": 0x"
                  << std::setw(2)
                  << static_cast<int>(regs[i])
                  << ((i % 4 == 3) ? '\n' : '\t');
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
