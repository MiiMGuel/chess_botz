#include <stdexcept>

#include "entry.hpp"
#include "SDL2/SDL.h"

class Game : public Program {
public:
    Game(int argc, char* argv[]) {
        if(SDL_Init(SDL_INIT_VIDEO) < 0) 
            throw std::runtime_error("SDL_Error: " + std::string(SDL_GetError()));
    }

    ~Game() override {
        SDL_Quit();
    }

    void Run(void) override {
        running = false;
    }
};

Program* main_program(int argc, char* argv[]) {
    Program* main = new Game(argc, argv);
    return main;
}