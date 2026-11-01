#ifndef BOARD_CONSTANTS_H
#define BOARD_CONSTANTS_H

enum {
    /**
     * @brief Number of bitboards used to represent pieces in a chess position.
     *
     * Includes 12 piece-specific bitboards, two color-specific occupancy
     * bitboards, and one combined occupancy bitboard.
     */
    BITBOARD_COUNT = 15,

    /**
     * @brief Maximum number of half-moves stored in a `BoardState::ply_count`.
     *
     * This is the theoretical upper bound on the number of plies in a chess
     * game under the [FIDE rules](https://handbook.fide.com/chapter/E012023).
     *
     * @see [arXiv:2608.14762 [math.GM]](https://arxiv.org/abs/2608.14762).
     */
    MAX_PLY_COUNT = 17697,

    /** @brief Sentinel value indicating that no en passant target exists. */
    NO_EN_PASSANT = 0xFF,
};

#endif
