#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <memory.h>

#include "logg.h"
#include "types.h"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

void board_setup(board_t* board, const char* fen) {
    // TODO: board setup func impl
}

void board_draw(
    SDL_Renderer* renderer, 
    SDL_Texture* pieces_img,
    const board_t* board,
    SDL_Color black, 
    SDL_Color white, 
    u32 whole_size
) {
    u32 square_size = whole_size / 8;
    for (u8 rank = 0; rank < 8; rank++) {
        for (u8 file = 0; file < 8; file++) {
            if ((rank + file) % 2 != 0) SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
            else  SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);

            SDL_FRect dst = {
                .w = square_size, 
                .h = square_size, 
                .x = 40 + square_size * file, 
                .y = 40 + square_size * rank
            }; SDL_RenderFillRect(renderer, &dst);
        }
    }
}