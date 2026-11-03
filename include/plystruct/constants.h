#ifndef PLYSTRUCT_CONSTANTS_H
#define PLYSTRUCT_CONSTANTS_H

enum {
    /**
     * @brief Maximum buffer size required to store a chess ply in UCI notation.
     *
     * Includes sufficient space to encode `from` and `to` squares, promotion
     * piece and the terminating null character.
     */
    UCI_PLY_BUFFER_SIZE = 6,
};

#endif
