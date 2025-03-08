#ifndef BOARD_H
#define BOARD_H

#include "types.h"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

#define BLACK_AC 0
#define WHITE_AC 1

typedef enum piece_id {
    PIECE_NULL         = 0x00,
    PIECE_BLACK_KING   = 0x01,
    PIECE_BLACK_QWEEN  = 0x02,
    PIECE_BLACK_ROOK   = 0x03,
    PIECE_BLACK_BISHOP = 0x04,
    PIECE_BLACK_KNIGHT = 0x05,
    PIECE_BLACK_PAWN   = 0x06,
    PIECE_WHITE_KING   = 0x11,
    PIECE_WHITE_QWEEN  = 0x12,
    PIECE_WHITE_ROOK   = 0x13,
    PIECE_WHITE_BISHOP = 0x14,
    PIECE_WHITE_KNIGHT = 0x15,
    PIECE_WHITE_PAWN   = 0x16
} piece_id_t;

typedef struct board {
    bool ac;
    bool bkc;
    bool bqc;
    bool wkc;
    bool wqc;
    bool ep;
    u32 hm;
    u32 fm;
    u8 square[64];
    char fen[128];
} board_t;

void board_setup(board_t* board, const char* fen);
void board_draw(
    SDL_Renderer* renderer, 
    SDL_Texture* pieces_img,
    TTF_Font* font, 
    const board_t* board, 
    SDL_Color black, 
    SDL_Color white, 
    u32 whole_size,
    bool draw_coord
);

#endif // BOARD_H