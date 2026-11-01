#ifndef BOARD_ENUMS_SQUARE_KIND_H
#define BOARD_ENUMS_SQUARE_KIND_H

#include "include/aliases/uints.h"

/**
 * @brief Piece kind occupying a square on a chess board.
 *
 * Encodes the six chess piece types for each color. Piece values range from
 * `0x00` to `0x0B` and correspond directly to their indices in
 * `BoardState::bitboards`. White pieces occupy even-indexed positions, while
 * black pieces occupy odd-indexed positions.
 */
typedef enum : U8 {
    /** @brief White pawns, mapped to a bitboard at index `0x00`. */
    WHITE_PAWN      = 0x00,

    /** @brief Black pawns, mapped to a bitboard at index `0x01`. */
    BLACK_PAWN      = 0x01,

    /** @brief White knights, mapped to a bitboard at index `0x02`. */
    WHITE_KNIGHT    = 0x02,

    /** @brief Black knights, mapped to a bitboard at index `0x03`. */
    BLACK_KNIGHT    = 0x03,

    /** @brief White bishops, mapped to a bitboard at index `0x04`. */
    WHITE_BISHOP    = 0x04,

    /** @brief Black bishops, mapped to a bitboard at index `0x05`. */
    BLACK_BISHOP    = 0x05,

    /** @brief White rooks, mapped to a bitboard at index `0x06`. */
    WHITE_ROOK      = 0x06,

    /** @brief Black rooks, mapped to a bitboard at index `0x07`. */
    BLACK_ROOK      = 0x07,

    /** @brief White queens, mapped to a bitboard at index `0x08`. */
    WHITE_QUEEN     = 0x08,

    /** @brief Black queens, mapped to a bitboard at index `0x09`. */
    BLACK_QUEEN     = 0x09,

    /** @brief White kings, mapped to a bitboard at index `0x0A`. */
    WHITE_KING      = 0x0A,

    /** @brief Black kings, mapped to a bitboard at index `0x0B`. */
    BLACK_KING      = 0x0B,
} PieceKind;

#endif
