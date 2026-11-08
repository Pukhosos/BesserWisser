#ifndef PLYSTRUCT_UNPLY_H
#define PLYSTRUCT_UNPLY_H

#include "include/aliases/uints.h"

/**
 * @brief 32-bit encoded reversed chess ply.
 *
 * Information is packed into a 32-bit integer in the following way:
 *
 * ```
 * Bit:   V U T S R Q P O N M L K J I H G | F E D C B A 9 8 | 7 6 5 4 | 3 2 1 0
 *        ---------------------------------------------------------------------
 * Value: c c c c c c c c c c c c c c c c | e e e e e e e e | x x x x | q Q k K
 * ```
 *
 * where:
 *
 *     1. Bits 0-3 encode previous castling rights (bits `q`, `Q`, `k`, `K`).
 *
 *     2. Bits 4-7 encode the captured piece (bits `x`).
 *
 *     3. Bits 8-F encode the previous en passant target (bits `e`).
 *
 *     4. Bits G-V encode the previous ply clock (bits `c`).
 *
 * Captured piece ncodes one the six chess piece types. Piece values range from
 * `0x00` to `0x0B` and correspond directly to their indices in
 * `BoardState::bitboards`. White pieces occupy even-indexed positions, while
 * black pieces occupy odd-indexed positions.
 *
 * En passant encodes the previous en passant target square, or
 * `NO_EN_PASSANT == 0xFF` if there was none.
 */
typedef U32 Unply;

/**
 * @brief Make a 32-bit encoded reversed chess ply.
 *
 * @param castling_rights Previous castling rights bitmask.
 * @param captured_piece Captured piece identifier, or `0xF` if no piece was
 * captured.
 * @param en_passant Previous en passant target square (0-63), or
 * `NO_EN_PASSANT = 0xFF` if no en passant target existed.
 * @param ply_clock Previous halfmove clock.
 *
 * @return 32-bit encoded reversed chess ply.
 *
 * ```
 * Bit:   V U T S R Q P O N M L K J I H G | F E D C B A 9 8 | 7 6 5 4 | 3 2 1 0
 *        ---------------------------------------------------------------------
 * Value: c c c c c c c c c c c c c c c c | e e e e e e e e | x x x x | q Q k K
 * ```
 *
 * where:
 *
 *     1. Bits 0-3 encode previous castling rights (bits `q`, `Q`, `k`, `K`).
 *
 *     2. Bits 4-7 encode the captured piece (bits `x`).
 *
 *     3. Bits 8-F encode the previous en passant target (bits `e`).
 *
 *     4. Bits G-V encode the previous ply clock (bits `c`).
 */
static inline Unply make_unply(
    U8 castling_rights,
    U8 captured_piece,
    U8 en_passant,
    U16 ply_clock
) {
    return (
        (U32)ply_clock << 16
        | (U32)en_passant << 8
        | (U32)captured_piece << 4
        | (U32)castling_rights
    );
}

#endif
