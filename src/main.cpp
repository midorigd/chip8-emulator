#include "chip8.h"
#include "platform.h"
#include "utils.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

using steady_clock = std::chrono::steady_clock;
using ms = std::chrono::duration<double, std::milli>;

namespace {
    constexpr int DEFAULT_SCALE { 20 };
}

int main(int argc, char** argv) {
    // platformTest();

    if (!(argc == 3 || argc == 4)) {
        std::cout << "Usage: ./chip8 <clock speed (Hz)> <ROM filename> [scale]\n";
        exit(1);
    }

    int delay { 1000 / std::stoi(argv[1]) };
    std::string romFilename { argv[2] };
    int scale { (argc == 4) ? std::stoi(argv[3]) : DEFAULT_SCALE };

    CHIP8 chip;
    Platform platform("CHIP8Window", scale);

    chip.loadROM(romFilename);

    // chip.printROM(0x200, 18);

    bool quit { false };
    auto lastCycleTime { steady_clock::now() };
    int dispPitch { sizeof(*chip.getDisplay()) * CHIP8::DISP_WIDTH };

    while (!quit) {
        quit = platform.processInput(chip.getKeyMap());
        auto currTime { steady_clock::now() };

        if (ms(currTime - lastCycleTime).count() >= delay) {
            chip.runCycle();
            platform.update(chip.getDisplay(), dispPitch);
            lastCycleTime = currTime;
        }
    }
}
