#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "include/aliases/uints.h"
#include "include/board/constants.h"
#include "include/board/enums/castling_right.h"
#include "include/board/enums/occupancy_kind.h"
#include "include/board/enums/piece_kind.h"
#include "include/board/enums/square.h"
#include "include/board/fen/constants.h"
#include "include/board/fen/enums/serialization_result.h"
#include "include/board/fen/interface.h"
#include "include/board/state.h"

/**
 * @brief Convert `PieceKind` into a FEN piece character.
 *
 * @return The corresponding FEN piece character (`\0` if invalid).
 */
static char char_from_piece(PieceKind piece) {
    switch (piece) {
        case WHITE_PAWN:    return 'P';
        case BLACK_PAWN:    return 'p';
        case WHITE_KNIGHT:  return 'N';
        case BLACK_KNIGHT:  return 'n';
        case WHITE_BISHOP:  return 'B';
        case BLACK_BISHOP:  return 'b';
        case WHITE_ROOK:    return 'R';
        case BLACK_ROOK:    return 'r';
        case WHITE_QUEEN:   return 'Q';
        case BLACK_QUEEN:   return 'q';
        case WHITE_KING:    return 'K';
        case BLACK_KING:    return 'k';
        default: return '\0';
    }
}

FenSerializationResult to_fen(const BoardState *board_state, char *fen) {
    if (board_state == NULL) return FEN_SERIALIZATION_NULL_BOARD_STATE;
    if (fen == NULL) return FEN_SERIALIZATION_NULL_FEN;
    char temp[FEN_BUFFER_SIZE];
    size_t length = 0;
    U64 combined_occupancy = board_state->bitboards[COMBINED_OCCUPANCY];

    // Field 1: Piece placement
    for (int rank = 7; rank >= 0; --rank) {
        int empty_squares = 0;
        for (int file = 0; file < 8; ++file) {
            Square square = (Square)(rank * 8 + file);
            SquareMask mask = make_square_mask_from_square(square);
            if (!(combined_occupancy & mask)) {
                ++empty_squares;
                continue;
            }
            if (empty_squares != 0) {
                temp[length++] = (char)('0' + empty_squares);
                empty_squares = 0;
            }
            for (PieceKind piece = WHITE_PAWN; piece <= BLACK_KING; ++piece) {
                if (board_state->bitboards[piece] & mask) {
                    temp[length++] = char_from_piece(piece);
                    break;
                }
            }
        }
        if (empty_squares != 0) temp[length++] = (char)('0' + empty_squares);
        if (rank != 0) temp[length++] = '/';
    }

    // Field 2: Side to move
    bool black_to_move = (board_state->ply_count & 1) != 0;
    temp[length++] = ' ';
    temp[length++] = black_to_move ? 'b' : 'w';

    // Field 3: Castling rights
    temp[length++] = ' ';
    U8 castling_rights = board_state->castling_rights;
    if (castling_rights == 0) temp[length++] = '-';
    else {
        if (castling_rights & CAN_WHITE_CASTLE_SHORT) temp[length++] = 'K';
        if (castling_rights & CAN_WHITE_CASTLE_LONG) temp[length++] = 'Q';
        if (castling_rights & CAN_BLACK_CASTLE_SHORT) temp[length++] = 'k';
        if (castling_rights & CAN_BLACK_CASTLE_LONG) temp[length++] = 'q';
    }

    // Field 4: En passant target
    temp[length++] = ' ';
    if (board_state->en_passant == NO_EN_PASSANT) {
        temp[length++] = '-';
    } else {
        Square square = (Square)board_state->en_passant;
        U8 rank = square / 8;
        U8 file = square % 8;
        temp[length++] = (char)('a' + file);
        temp[length++] = (char)('1' + rank);
    }

    // Field 5: Halfmove clock
    int written_half_move_clock = snprintf(
        temp + length,
        FEN_BUFFER_SIZE - length,
        " %u",
        (unsigned int)board_state->ply_clock
    );
    if (
        written_half_move_clock < 0
    ) return FEN_SERIALIZATION_HALFMOVE_CLOCK_FORMATTING_ERROR;
    if (
        (size_t)written_half_move_clock >= sizeof(temp) - length
    ) return FEN_SERIALIZATION_BUFFER_TOO_SMALL;
    length += (size_t)written_half_move_clock;

    // Field 6: Fullmove number
    U16 fullmove_number = (U16)board_state->ply_count / (U16)2 + (U16)1;
    int written_fullmove_number = snprintf(
        temp + length,
        FEN_BUFFER_SIZE - length,
        " %u",
        (unsigned int)fullmove_number
    );
    if (
        written_fullmove_number < 0
    ) return FEN_SERIALIZATION_FULLMOVE_NUMBR_FORMATTING_ERROR;
    if (
        (size_t)written_fullmove_number >= FEN_BUFFER_SIZE - length
    ) return FEN_SERIALIZATION_BUFFER_TOO_SMALL;
    length += (size_t)written_fullmove_number;

    // Commit the fully serialized FEN string
    memcpy(fen, temp, length + 1);
    return FEN_SERIALIZATION_OK;
}
