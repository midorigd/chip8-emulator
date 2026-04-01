#include "platform.h"
#include "chip8.h"

#include <iostream>
#include <SDL.h>

void Platform::test() {
    std::cout << "testing platform compilation\n";
}

Platform::Platform() {
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
    window = SDL_CreateWindow("CHIP8Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, CHIP8::DISP_WIDTH, CHIP8::DISP_HEIGHT);
}

Platform::~Platform() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Platform::update(const void* pixels, int pitch) {
    // refresh texture data
    SDL_UpdateTexture(texture, nullptr, pixels, pitch);

    // render new texture
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);

    // display updated rendering
    SDL_RenderPresent(renderer);
}

bool Platform::processInput(bool* keyMap) {
    bool quit { false };
    SDL_Event currEvent;

    while (SDL_PollEvent(&currEvent)) {
        bool keyVal { false };

        switch (currEvent.type) {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_KEYDOWN:
                keyVal = true;
                [[fallthrough]];

            case SDL_KEYUP:
                switch (currEvent.key.keysym.sym) {
                    case SDLK_1: keyMap[0x1] = keyVal; break;
                    case SDLK_2: keyMap[0x2] = keyVal; break;
                    case SDLK_3: keyMap[0x3] = keyVal; break;
                    case SDLK_4: keyMap[0xC] = keyVal; break;
                    case SDLK_q: keyMap[0x4] = keyVal; break;
                    case SDLK_w: keyMap[0x5] = keyVal; break;
                    case SDLK_e: keyMap[0x6] = keyVal; break;
                    case SDLK_r: keyMap[0xD] = keyVal; break;
                    case SDLK_a: keyMap[0x7] = keyVal; break;
                    case SDLK_s: keyMap[0x8] = keyVal; break;
                    case SDLK_d: keyMap[0x9] = keyVal; break;
                    case SDLK_f: keyMap[0xE] = keyVal; break;
                    case SDLK_z: keyMap[0xA] = keyVal; break;
                    case SDLK_x: keyMap[0x0] = keyVal; break;
                    case SDLK_c: keyMap[0xB] = keyVal; break;
                    case SDLK_v: keyMap[0xF] = keyVal; break;
                }
                break;
        }

        return quit;
    }
}
