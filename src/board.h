#ifndef BOARD_H
#define BOARD_H

#include "types.h"
#include "SDL3/SDL.h"

typedef enum piece_id {
    PIECE_NULL         = 0x00,
    PIECE_SELECTED     = 0x01,
    PIECE_BLACK_KING   = 0x02,
    PIECE_BLACK_QWEEN  = 0x03,
    PIECE_BLACK_ROOK   = 0x04,
    PIECE_BLACK_BISHOP = 0x05,
    PIECE_BLACK_KNIGHT = 0x06,
    PIECE_BLACK_PAWN   = 0x07,
    PIECE_WHITE_KING   = 0x12,
    PIECE_WHITE_QWEEN  = 0x13,
    PIECE_WHITE_ROOK   = 0x14,
    PIECE_WHITE_BISHOP = 0x15,
    PIECE_WHITE_KNIGHT = 0x16,
    PIECE_WHITE_PAWN   = 0x17
} piece_id_t;

typedef struct board {
    bool ac;
    bool bkq;
    bool wkq;
    bool ep;
    u8 square[64];
    char fen[128];
} board_t;

void board_draw(SDL_Renderer* renderer, SDL_Texture* pieces_img, const board_t* board, SDL_Color black, SDL_Color white, u32 whole_size);

#endif // BOARD_H