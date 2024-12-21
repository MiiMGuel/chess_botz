#include <stdexcept>

#define ENTRY_IMPL
#include "engine/types.hpp"
#include "engine/entry.hpp"
#include "SDL2/SDL.h"

class Game : public Program {
private:
    SDL_Window* window = nullptr;
    SDL_Event   event  = { 0 };

public:
    Game(int argc, char* argv[]) {
        if (SDL_Init(SDL_INIT_VIDEO)) 
            throw std::runtime_error(SDL_GetError());

        window = SDL_CreateWindow(
            "game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            640, 480, SDL_WINDOW_RESIZABLE
        ); if (window == nullptr) throw std::runtime_error(SDL_GetError());
    }

    ~Game() override {
        if (window) SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void Run(void) override {
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT) running = false;
    }
};

Program* MainProgram(int argc, char* argv[]) {
    Program* main = new Game(argc, argv);
    return main;
}