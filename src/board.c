#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <memory.h>

#include "logg.h"
#include "types.h"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "SDL_ext.h"

void board_setup(board_t* board, const char* fen) {
    // Clear the board
    memset(board->square, PIECE_NULL, 64);

    // Copy the FEN string to the board structure
    strncpy(board->fen.string, fen, sizeof(board->fen.string) - 1);
    board->fen.string[sizeof(board->fen.string) - 1] = '\0';

    // Parse the FEN string
    int index = 0;
    int square = 0;

    // Piece placement
    while (fen[index] != ' ' && square < 64) {
        char c = fen[index];
        if (isdigit(c)) {
            int emptySquares = c - '0';
            square += emptySquares;
        } else if (c == '/') {
            // Do nothing, just move to the next rank
        } else {
            switch (c) {
                case 'k': board->square[square] = PIECE_BLACK_KING; break;
                case 'q': board->square[square] = PIECE_BLACK_QWEEN; break;
                case 'r': board->square[square] = PIECE_BLACK_ROOK; break;
                case 'b': board->square[square] = PIECE_BLACK_BISHOP; break;
                case 'n': board->square[square] = PIECE_BLACK_KNIGHT; break;
                case 'p': board->square[square] = PIECE_BLACK_PAWN; break;
                case 'K': board->square[square] = PIECE_WHITE_KING; break;
                case 'Q': board->square[square] = PIECE_WHITE_QWEEN; break;
                case 'R': board->square[square] = PIECE_WHITE_ROOK; break;
                case 'B': board->square[square] = PIECE_WHITE_BISHOP; break;
                case 'N': board->square[square] = PIECE_WHITE_KNIGHT; break;
                case 'P': board->square[square] = PIECE_WHITE_PAWN; break;
                default: break;
            }
            square++;
        }
        index++;
    }

    // Active color
    index++; // Skip the space
    board->fen.active_color = (fen[index] == 'w');

    // Castling availability
    index += 2; // Skip the space and the active color character
    board->fen.black_king_castle = false;
    board->fen.black_qween_castle = false;
    board->fen.white_king_castle = false;
    board->fen.white_qween_castle = false;
    while (fen[index] != ' ') {
        switch (fen[index]) {
            case 'k': board->fen.black_king_castle = true; break;
            case 'q': board->fen.black_qween_castle = true; break;
            case 'K': board->fen.white_king_castle = true; break;
            case 'Q': board->fen.white_qween_castle = true; break;
            default: break;
        }
        index++;
    }

    // En passant target square
    index++; // Skip the space
    if (fen[index] != '-') {
        board->fen.en_passant[0] = fen[index];
        board->fen.en_passant[1] = fen[index + 1];
        index += 2;
    } else {
        board->fen.en_passant[0] = '\0';
        board->fen.en_passant[1] = '\0';
        index++;
    }

    // Halfmove clock
    index++; // Skip the space
    board->fen.halfmove_clock = 0;
    while (isdigit(fen[index])) {
        board->fen.halfmove_clock = board->fen.halfmove_clock * 10 + (fen[index] - '0');
        index++;
    }

    // Fullmove number
    index++; // Skip the space
    board->fen.fullmove_number = 0;
    while (isdigit(fen[index])) {
        board->fen.fullmove_number = board->fen.fullmove_number * 10 + (fen[index] - '0');
        index++;
    }
}

void board_draw(
    SDL_Renderer* renderer, 
    SDL_Texture* board_img,
    SDL_Texture* pieces_img,
    const board_t* board,
    u32 size
) {
    u32 square_size = size / 8;
    u32 coord_size  = square_size / 4;
    u32 index       = 0;

    SDL_RenderTexture(renderer, board_img, NULL, &(SDL_FRect){.w = size, .h = size, .x = 40, .y = 40});
    for (u8 rank = 0; rank < 8; rank++) {
        for (u8 file = 0; file < 8; file++) {

            SDL_FRect dst = {
                .w = square_size, 
                .h = square_size, 
                .x = 40 + square_size * file, 
                .y = 40 + square_size * rank
            };

            switch (board->square[index]) {
                case PIECE_BLACK_KING: SDL_RenderTexture(renderer, pieces_img, &(SDL_FRect){.w=200,.h=200,.x=0,.y=200}, &dst); break;
                case PIECE_BLACK_QWEEN: SDL_RenderTexture(renderer, pieces_img, &(SDL_FRect){.w=200,.h=200,.x=200,.y=200}, &dst); break;
                case PIECE_BLACK_ROOK: SDL_RenderTexture(renderer, pieces_img, &(SDL_FRect){.w=200,.h=200,.x=800,.y=200}, &dst); break;
                case PIECE_BLACK_BISHOP: SDL_RenderTexture(renderer, pieces_img, &(SDL_FRect){.w=200,.h=200,.x=400,.y=200}, &dst); break;
                case PIECE_BLACK_KNIGHT: SDL_RenderTexture(renderer, pieces_img, &(SDL_FRect){.w=200,.h=200,.x=600,.y=200}, &dst); break;
                case PIECE_BLACK_PAWN: SDL_RenderTexture(renderer, pieces_img, &(SDL_FRect){.w=200,.h=200,.x=1000,.y=200}, &dst); break;
                case PIECE_WHITE_KING: SDL_RenderTexture(renderer, pieces_img, &(SDL_FRect){.w=200,.h=200,.x=0,.y=0}, &dst); break;
                case PIECE_WHITE_QWEEN: SDL_RenderTexture(renderer, pieces_img, &(SDL_FRect){.w=200,.h=200,.x=200,.y=0}, &dst); break;
                case PIECE_WHITE_ROOK: SDL_RenderTexture(renderer, pieces_img, &(SDL_FRect){.w=200,.h=200,.x=800,.y=0}, &dst); break;
                case PIECE_WHITE_BISHOP: SDL_RenderTexture(renderer, pieces_img, &(SDL_FRect){.w=200,.h=200,.x=400,.y=0}, &dst); break;
                case PIECE_WHITE_KNIGHT: SDL_RenderTexture(renderer, pieces_img, &(SDL_FRect){.w=200,.h=200,.x=600,.y=0}, &dst); break;
                case PIECE_WHITE_PAWN: SDL_RenderTexture(renderer, pieces_img, &(SDL_FRect){.w=200,.h=200,.x=1000,.y=0}, &dst); break;
                default: break;
            } index++;
        }
    }
}