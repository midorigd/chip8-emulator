#ifndef PLATFORM_H
#define PLATFORM_H

#include "chip8.h"

#include <SDL.h>
#include <string>

class Platform {
public:
    static void test();

    // Creates a new SDL window with the given window title and scaling factor
    Platform(const std::string& windowTitle, unsigned scale);

    // Cleanup for the SDL platform attributes
    ~Platform();

    // Updates the display rendering based on the display array and pitch
    void update(const void* pixels, int pitch);

    // Reads keyboard input from the user and stores it into the keymap array
    bool processInput(bool* keyMap);

private:
    // platform parameters
    // static constexpr unsigned DEFAULT_SCALE { 20 };
    // static constexpr unsigned WINDOW_WIDTH { CHIP8::DISP_WIDTH * DEFAULT_SCALE };
    // static constexpr unsigned WINDOW_HEIGHT { CHIP8::DISP_HEIGHT * DEFAULT_SCALE };

    // SDL platform attributes
    SDL_Window* window {};
    SDL_Renderer* renderer {};
    SDL_Texture* texture {};
};

#endif
