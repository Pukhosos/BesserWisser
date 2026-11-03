#ifndef BOARD_ENUMS_OCCUPANCY_KIND_H
#define BOARD_ENUMS_OCCUPANCY_KIND_H

#include "include/aliases/uints.h"

/**
 * @brief Aggregate occupancy bitboard indices.
 *
 * Values correspond directly to indices in `BoardState::bitboards`, following
 * the 12 individual piece bitboards. `WHITE_OCCUPANCY` and `BLACK_OCCUPANCY`
 * represent all squares occupied by the respective color, `COMBINED_OCCUPANCY`
 * represents all occupied squares regardless of color.
 */
typedef enum : U8 {
    /**
     * @brief All squares occupied by white pieces, mapped to a bitboard with
     * index `0x0C`.
     */
    WHITE_OCCUPANCY     = 0x0C,

    /**
     * @brief All squares occupied by black pieces, mapped to a bitboard with
     * index `0x0D`.
     */
    BLACK_OCCUPANCY     = 0x0D,

    /**
     * @brief All occupied squares regardless of piece color, mapped to a
     * bitboard with index `0x0E`.
     */
    COMBINED_OCCUPANCY  = 0x0E,
} OccupancyKind;

#endif
