#ifndef BOARD_PLY_H
#define BOARD_PLY_H

#include "include/board/state.h"
#include "include/plystruct/ply.h"
#include "include/plystruct/unply.h"

/**
 * @brief Apply a chess ply to the given board state.
 *
 * @param board_state Pointer to the board state to modify.
 * @param ply 16-bit encoded chess ply to apply.
 *
 * @return 32-bit encoded reversed chess ply containing the information needed
 * to restore the previous board state.
 *
 * Updates the piece bitboards, castling rights, en passant target, halfmove
 * clock, and ply count.
 *
 * @note The function modifies `board_state` in place.
 */
Unply apply_ply(BoardState* board_state, Ply ply);

#endif
