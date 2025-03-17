#ifndef BOARD_H
#define BOARD_H

#include "types.h"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

#define BLACK_TURN 0
#define WHITE_TURN 1

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

typedef struct pgn { // Portable Game Notation (PGN)
    char string[1024];
    char event[64];
    char date[64];
    char time[64];
    char white_name[64];
    char black_name[64];
    i32 white_elo;
    i32 black_elo;
} pgn_t;

typedef struct fen { // Forsyth-Edwards Notation (FEN)
    char string[128];
    bool active_color;
    bool black_king_castle;
    bool black_qween_castle;
    bool white_king_castle;
    bool white_qween_castle;
    char en_passant[2];
    u32 halfmove_clock;
    u32 fullmove_number;
} fen_t;

typedef struct mhn { // Move history Notation (MHN)
    
} mhn_t;

typedef struct board {
    u8 square[64];
    pgn_t pgn;
    fen_t fen;
} board_t;

void board_setup(board_t* board, const char* fen);
void board_draw(
    SDL_Renderer* renderer, 
    SDL_Texture* board_img,
    SDL_Texture* pieces_img,
    const board_t* board, 
    u32 size
);

#endif // BOARD_H