#include <stdexcept>

#include "engine/engine.hpp"
#include "engine/entry.hpp"
#include "SDL2/SDL.h"

class Game : public Engine::Program {
private:
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event     event    = { 0 };

public:
    Game(int argc, char* argv[]) {
        Engine::Init();

        window = SDL_CreateWindow(
            "game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN
        ); if (window == nullptr) throw std::runtime_error(SDL_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr) throw std::runtime_error(SDL_GetError());
        SDL_ShowWindow(window);
    }

    ~Game() override {
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        Engine::Close();
    }

    void Run(void) override {
        static SDL_Rect rect = (SDL_Rect){.x = 640 / 2, .y = 480 / 2, .w = 50, .h = 50};

        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT) running = false;
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);
    }
};

Engine::Program* Engine::MainProgram(int argc, char* argv[]) {
    Engine::Program* main = new Game(argc, argv);
    return main;
}