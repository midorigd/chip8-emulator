#ifndef PLATFORM_H
#define PLATFORM_H

#include "chip8.h"

#include <SDL.h>

class Platform {
public:
    static void test();

    // constructor
    Platform();

    // destructor
    ~Platform();

    // update
    // processinput

private:
    // platform parameters
    static constexpr unsigned SCALE { 20 };
    static constexpr unsigned WINDOW_WIDTH { CHIP8::DISP_WIDTH * SCALE };
    static constexpr unsigned WINDOW_HEIGHT { CHIP8::DISP_HEIGHT * SCALE };

    // window
    SDL_Window* window;

    // renderer
    SDL_Renderer* renderer;

    // texture
    SDL_Texture* texture;
};

#endif
