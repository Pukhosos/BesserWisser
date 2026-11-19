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

/**
 * @brief Undo a previously applied chess ply.
 *
 * @param board_state Pointer to the board state to restore.
 * @param ply 16-bit encoded chess ply to undo.
 * @param unply 32-bit encoded reversed chess ply returned by the corresponding
 * call to `apply_ply`.
 *
 * Restores the board state to its exact configuration before `ply` was applied.
 * The provided `ply` and `unply` must correspond to the most recently applied
 * ply. Plies must be undone in reverse order of application.
 *
 * @note The function modifies `board_state` in place.
 */
void undo_ply(BoardState* board_state, Ply ply, Unply unply);

#endif
