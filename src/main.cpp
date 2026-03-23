#include "chip8.h"
#include "utils.h"

#include <fstream>
#include <iostream>

int main() {
    createTestBin("test.ch8");

    CHIP8 chip;
    chip.loadROM("test.ch8");

    chip.printROM(0x50, 82);
    chip.printROM(0x200, 18);
    chip.printRegs();
}
