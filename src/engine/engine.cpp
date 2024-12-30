#include <stdexcept>

#include "engine.hpp"
#include "SDL2/SDL.h"

namespace Engine {
    static u32 init = 0;
    
    void Init(void) {
        if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) 
            throw std::runtime_error(SDL_GetError());

        init = 1;
    }

    void Close(void) {
        if (init)
            SDL_Quit();
        
        init = 0;
    }
}