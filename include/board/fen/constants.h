#ifndef BOARD_FEN_CONSTANTS_H
#define BOARD_FEN_CONSTANTS_H

enum {
    /**
     * @brief Maximum buffer size required to store a FEN string.
     *
     * Includes sufficient space for all six FEN fields and the terminating
     * null character.
     *
     * @note Assumes a valid board state with a ply count not exceeding
     * `MAX_PLY_COUNT = 17697`.
     */
    FEN_BUFFER_SIZE = 96,
};

#endif
