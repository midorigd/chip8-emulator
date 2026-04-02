#include "utils.h"
#include "chip8.h"
#include "platform.h"

#include <fstream>
#include <iostream>
#include <SDL.h>
#include <string>
#include <vector>

void createTestBin(const std::string& filename) {
    std::vector<long> vals { 0xABCD1234, 0x567890EF, 0x83B4AC1F, 0xF72B1AC8 };
    std::ofstream binFile(filename, std::ios::binary);

    if (!binFile) {
        std::cerr << "Test binary file " << filename << " not opened\n";
        exit(1);
    }

    for (long val : vals) {
        binFile.write(reinterpret_cast<char*>(&val), 4);
    }

    binFile.close();
}

void chipTest() {
    createTestBin("test.ch8");

    CHIP8 chip;
    chip.loadROM("test.ch8");

    chip.printROM(0x50, 82);
    chip.printROM(0x200, 18);
    chip.printRegs();

    Platform::test();
}

void platformTest() {
    Platform platform("testWindow", 20);
    uint32_t testDisplay[CHIP8::DISP_WIDTH * CHIP8::DISP_HEIGHT] {};

    for (int i = 0; i < CHIP8::DISP_WIDTH * CHIP8::DISP_HEIGHT; i++) {
        testDisplay[i] = ((i % 2) == (i / CHIP8::DISP_WIDTH % 2)) ? 0xFFFFFFFF : 0x00000000;
    }

    int dispPitch { sizeof(uint32_t) * CHIP8::DISP_WIDTH };
    platform.update(testDisplay, dispPitch);

    SDL_Event e;
    bool quit { false };
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
        }
    }
}
