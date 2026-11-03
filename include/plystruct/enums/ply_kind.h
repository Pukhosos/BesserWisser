#ifndef PLYSTRUCT_ENUMS_PLY_KIND_H
#define PLYSTRUCT_ENUMS_PLY_KIND_H

#include "include/aliases/uints.h"

/**
 * @brief Ply kind encoded in bits E-F.
 *
 * Bits `k` on this diagram:
 *
 * ```
 * Bit:   F E | D C | B A 9 8 7 6 | 5 4 3 2 1 0
 *        -------------------------------------
 * Value: k k | - - | - - - - - - | - - - - - -
 * ```
 */
typedef enum : U16 {
    /**
     * @brief Encodes an ordinary ply.
     *
     * Relevant ply kind bits:
     *
     * ```
     * Bit:   F E | D C | B A 9 8 7 6 | 5 4 3 2 1 0
     *        -------------------------------------
     * Value: 0 0 | - - | - - - - - - | - - - - - -
     * ```
     */
    ORDINARY_MOVE    = 0x0000,

    /**
     * @brief Encodes a pawn promotion.
     *
     * Relevant ply kind bits:
     *
     * ```
     * Bit:   F E | D C | B A 9 8 7 6 | 5 4 3 2 1 0
     *        -------------------------------------
     * Value: 0 1 | - - | - - - - - - | - - - - - -
     * ```
     */
    PROMOTION_MOVE   = 0x4000,

    /**
     * @brief Encodes an en passant capture.
     *
     * Relevant ply kind bits:
     *
     * ```
     * Bit:   F E | D C | B A 9 8 7 6 | 5 4 3 2 1 0
     *        -------------------------------------
     * Value: 1 0 | - - | - - - - - - | - - - - - -
     * ```
     */
    EN_PASSANT_MOVE  = 0x8000,

    /**
     * @brief Encodes castling.
     *
     * Relevant ply kind bits:
     *
     * ```
     * Bit:   F E | D C | B A 9 8 7 6 | 5 4 3 2 1 0
     *        -------------------------------------
     * Value: 1 1 | - - | - - - - - - | - - - - - -
     * ```
     */
    CASTLING_MOVE    = 0xC000,
} PlyKind;

#endif
