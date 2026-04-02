#include "chip8.h"
#include "platform.h"
#include "utils.h"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: ./chip8 <clock speed (Hz)> <ROM filename>\n";
        exit(1);
    }

    int delay { 1000 / std::stoi(argv[1]) };
    std::string romFilename { argv[2] };

    CHIP8 chip;
    Platform platform("CHIP8Window");

    chip.loadROM(romFilename);

    bool quit { false };

    while (!quit) {
        quit = platform.processInput(chip.getKeyMap());

        chip.runCycle();
    }
}
