#ifndef BOARD_ENUMS_CASTLING_RIGHT_H
#define BOARD_ENUMS_CASTLING_RIGHT_H

#include "include/aliases/uints.h"

/**
 * @brief Bit flags describing the castling rights.
 *
 * ```
 * Bit:   7 6 5 4 3 2 1 0
 *        ---------------
 * Value: - - - - q Q k K
 * ```
 *
 * @note Bits 4-7 are reserved and should be zero.
 */
typedef enum : U8 {
    /**
     * @brief Grants white the right to castle kingside.
     *
     * Represented by bit `K`:
     * ```
     * Bit:   7 6 5 4 3 2 1 0
     *        ---------------
     * Value: - - - - q Q k K
     * ```
     */
    CAN_WHITE_CASTLE_SHORT  = 1 << 0,

    /**
     * @brief Grants black the right to castle kingside.
     *
     * Represented by bit `k`:
     * ```
     * Bit:   7 6 5 4 3 2 1 0
     *        ---------------
     * Value: - - - - q Q k K
     * ```
     */
    CAN_BLACK_CASTLE_SHORT  = 1 << 1,

    /**
     * @brief Grants white the right to castle queenside.
     *
     * Represented by bit `Q`:
     * ```
     * Bit:   7 6 5 4 3 2 1 0
     *        ---------------
     * Value: - - - - q Q k K
     * ```
     */
    CAN_WHITE_CASTLE_LONG   = 1 << 2,

    /**
     * @brief Grants black the right to castle queenside.
     *
     * Represented by bit `q`:
     * ```
     * Bit:   7 6 5 4 3 2 1 0
     *        ---------------
     * Value: - - - - q Q k K
     * ```
     */
    CAN_BLACK_CASTLE_LONG   = 1 << 3,
} CastlingRight;

#endif
