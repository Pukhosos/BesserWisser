#ifndef PLYSTRUCT_ENUMS_MASKS_H
#define PLYSTRUCT_ENUMS_MASKS_H

#include "include/aliases/uints.h"

typedef enum : U16 {
    /**
     * @brief Bitmask for extracting the ply kind section.
     *
     * Selects bits E-F of a 16-bit encoded ply.
     *
     * ```
     * Bit:   F E | D C | B A 9 8 7 6 | 5 4 3 2 1 0
     *        -------------------------------------
     * Mask:  1 1 | 0 0 | 0 0 0 0 0 0 | 0 0 0 0 0 0
     * ```
     */
    PLY_KIND_MASK = 0xC000,

    /**
     * @brief Bitmask for extracting the promotion piece section.
     *
     * Selects bits C-D of a 16-bit encoded ply.
     *
     * ```
     * Bit:   F E | D C | B A 9 8 7 6 | 5 4 3 2 1 0
     *        -------------------------------------
     * Mask:  0 0 | 1 1 | 0 0 0 0 0 0 | 0 0 0 0 0 0
     * ```
     */
    PROMOTION_KIND_MASK = 0x3000,

    /**
     * @brief Bitmask for extracting `from` square index.
     *
     * Selects bits 6-B of a 16-bit encoded ply.
     *
     * ```
     * Bit:   F E | D C | B A 9 8 7 6 | 5 4 3 2 1 0
     *        -------------------------------------
     * Mask:  0 0 | 0 0 | 1 1 1 1 1 1 | 0 0 0 0 0 0
     * ```
     */
    FROM_SQUARE_MASK = 0x0FC0,

    /**
     * @brief Bitmask for extracting `to` square index.
     *
     * Selects bits 0-5 of a 16-bit encoded ply.
     *
     * ```
     * Bit:   F E | D C | B A 9 8 7 6 | 5 4 3 2 1 0
     *        -------------------------------------
     * Mask:  0 0 | 0 0 | 0 0 0 0 0 0 | 1 1 1 1 1 1
     * ```
     */
    TO_SQUARE_MASK = 0x003F,
} PlySectionMask;

#endif
