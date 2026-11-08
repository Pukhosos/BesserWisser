#ifndef PLYGEN_INTERNAL_PSEUDOLEGAL_H
#define PLYGEN_INTERNAL_PSEUDOLEGAL_H

#include "include/board/state.h"
#include "include/plystruct/ply.h"

/**
 * @brief Generate pseudo-legal pawn plies for the given board state.
 *
 * @param board_state Pointer to the current board state.
 * @param plyarray Pointer to the first available buffer element.
 * @return Pointer to an element one past the last generated ply.
 *
 * @note Generated plies are written sequentially into the buffer starting at
 * `plyarray`.
 */
Ply* generate_pseudolegal_pawn_plies(
    const BoardState* board_state,
    Ply* plyarray
);

/**
 * @brief Generate pseudo-legal knight plies for the given board state.
 *
 * @param board_state Pointer to the current board state.
 * @param plyarray Pointer to the first available buffer element.
 * @return Pointer to an element one past the last generated ply.
 *
 * @note Generated plies are written sequentially into the buffer starting at
 * `plyarray`.
 */
Ply* generate_pseudolegal_knight_plies(
    const BoardState* board_state,
    Ply* plyarray
);

/**
 * @brief Generate pseudo-legal king plies for the given board state.
 *
 * @param board_state Pointer to the current board state.
 * @param plyarray Pointer to the first available buffer element.
 * @return Pointer to an element one past the last generated ply.
 *
 * @note Generated plies are written sequentially into the buffer starting at
 * `plyarray`.
 */
Ply* generate_pseudolegal_king_plies(
    const BoardState* board_state,
    Ply* plyarray
);

#endif
