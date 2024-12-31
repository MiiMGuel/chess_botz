#include <unordered_map>
#include <stdexcept>

#include "engine/engine.hpp"
#include "engine/entry.hpp"
#include "SDL2/SDL.h"

enum class InputType {
    KEYBOARD,
    MOUSE
};

class InputBind {
public:
    InputType type;
    union {
        i32 key;
        u8 mouseButton;
    };
    bool down = false;
    bool pressed = false;

    InputBind(void) : type(InputType::KEYBOARD), key(SDLK_UNKNOWN) { }
    InputBind(i32 k) : type(InputType::KEYBOARD), key(k) { }
    InputBind(u8 mb) : type(InputType::MOUSE), mouseButton(mb) { }
};

class InputHandler {
private:
    std::unordered_map<str, InputBind> input_map;
public:
    InputHandler(std::unordered_map<str, InputBind> map) : input_map(map) { }
    ~InputHandler() {
        input_map.clear();
    }

    void MapAddBind(str name, InputBind bind) {
        input_map.insert({name, bind});
    }

    void MapSetBind(str name, InputBind bind) {
        input_map[name] = bind;
    }

    void MapRemovebind(str name) {
        input_map.erase(name);
    }

    void MapUpdateEvent(const SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) {
            for (auto& imap : input_map) {
                if (imap.second.type == InputType::KEYBOARD && event.key.keysym.sym == imap.second.key) {
                    imap.second.down = true;
                    imap.second.pressed = true;
                }
            }
        } if (event.type == SDL_KEYUP) {
            for (auto& imap : input_map) {
                if (imap.second.type == InputType::KEYBOARD && event.key.keysym.sym == imap.second.key) {
                    imap.second.down = false;
                }
            }
        } if (event.type == SDL_MOUSEBUTTONDOWN) {
            for (auto& imap : input_map) {
                if (imap.second.type == InputType::MOUSE && event.button.button == imap.second.mouseButton) {
                    imap.second.down = true;
                    imap.second.pressed = true;
                }
            }
        } if (event.type == SDL_MOUSEBUTTONUP) {
            for (auto& imap : input_map) {
                if (imap.second.type == InputType::MOUSE && event.button.button == imap.second.mouseButton) {
                    imap.second.down = false;
                }
            }
        }
    }

    void MapResetEvent(void) {
        for (auto& imap : input_map) 
            imap.second.pressed = false;
    }

    const InputBind GetInput(str name) {
        return input_map[name];
    }
};

class Game : public Engine::Program {
private:
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event     event    = { 0 };
    InputHandler  input    = InputHandler({
        { "up",    InputBind(SDLK_w) },
        { "left",  InputBind(SDLK_a) },
        { "down",  InputBind(SDLK_s) },
        { "right", InputBind(SDLK_d) },
        { "click", InputBind((u8)SDL_BUTTON_LEFT) },
    });
    SDL_Rect rect          = SDL_Rect{
        .x = 640 / 2, 
        .y = 480 / 2, 
        .w = 50, 
        .h = 50
    };

public:
    Game(int argc, char* argv[]) {
        Engine::Init();

        window = SDL_CreateWindow(
            "game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN
        ); if (window == nullptr) throw std::runtime_error(SDL_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) throw std::runtime_error(SDL_GetError());
        SDL_ShowWindow(window);
    }

    ~Game() override {
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        Engine::Close();
    }

    void Run(void) override {
        input.MapResetEvent();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            input.MapUpdateEvent(event);
        }

        if (input.GetInput("up").down) rect.y -= 5;
        if (input.GetInput("left").down) rect.x -= 5;
        if (input.GetInput("down").down) rect.y += 5;
        if (input.GetInput("right").down) rect.x += 5;
        if (input.GetInput("click").down) SDL_GetMouseState(&rect.x, &rect.y);
        
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