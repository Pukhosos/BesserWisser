#ifndef PLYSTRUCT_ENUMS_PROMOTION_KIND_H
#define PLYSTRUCT_ENUMS_PROMOTION_KIND_H

#include "include/aliases/uints.h"

/**
 * @brief Promotion piece type encoded in bits C-D.
 *
 * Bits `p` on this diagram:
 *
 * ```
 * Bit:   F E | D C | B A 9 8 7 6 | 5 4 3 2 1 0
 *        -------------------------------------
 * Value: - - | p p | - - - - - - | - - - - - -
 * ```
 */
typedef enum : U16 {
    /**
     * @brief Promote a pawn to a knight.
     *
     * Relevant promotion bits:
     *
     * ```
     * Bit:   F E | D C | B A 9 8 7 6 | 5 4 3 2 1 0
     *        -------------------------------------
     * Value: - - | 0 0 | - - - - - - | - - - - - -
     * ```
     */
    KNIGHT_PROMOTION  = 0x0000,

    /**
     * @brief Promote a pawn to a bishop.
     *
     * Relevant promotion bits:
     *
     * ```
     * Bit:   F E | D C | B A 9 8 7 6 | 5 4 3 2 1 0
     *        -------------------------------------
     * Value: - - | 0 1 | - - - - - - | - - - - - -
     * ```
     */
    BISHOP_PROMOTION  = 0x1000,

    /**
     * @brief Promote a pawn to a rook.
     *
     * Relevant promotion bits:
     *
     * ```
     * Bit:   F E | D C | B A 9 8 7 6 | 5 4 3 2 1 0
     *        -------------------------------------
     * Value: - - | 1 0 | - - - - - - | - - - - - -
     * ```
     */
    ROOK_PROMOTION    = 0x2000,

    /**
     * @brief Promote a pawn to a queen.
     *
     * Relevant promotion bits:
     *
     * ```
     * Bit:   F E | D C | B A 9 8 7 6 | 5 4 3 2 1 0
     *        -------------------------------------
     * Value: - - | 1 1 | - - - - - - | - - - - - -
     * ```
     */
    QUEEN_PROMOTION   = 0x3000,
} PromotionKind;

#endif
