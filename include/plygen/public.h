#ifndef PLYGEN_PUBLIC_H
#define PLYGEN_PUBLIC_H

#include "include/board/state.h"
#include "include/plystruct/ply.h"

/**
 * @brief Generate all pseudo-legal plies for the given position.
 *
 * @param board_state Pointer to the current board state.
 * @param plyarray Pointer to to the first available buffer element.
 * @return  Pointer to an element one past the last generated ply.
 *
 * Pseudo-legal plies follow piece movement rules but may leave the moving
 * side king in check.
 *
 * Generated plies are written sequentially into the buffer starting at
 * `plyarray`. The function returns a pointer one past the last generated ply.
 *
 * For example, given the following buffer:
 *
 * ```
 *    -+----+----+----+----+----+----+----+----+----+----+----+-
 * ... | p0 | p1 |    |    |    |    |    |    |    |    |    | ...
 *    -+----+----+----+----+----+----+----+----+----+----+----+-
 *                 ^^
 *              plyarray
 * ```
 *
 * After generating five plies:
 *
 * ```
 *    -+----+----+----+----+----+----+----+----+----+----+----+-
 * ... | p0 | p1 | p2 | p3 | p4 | p5 | p6 |    |    |    |    | ...
 *    -+----+----+----+----+----+----+----+----+----+----+----+-
 *                                          ^^
 *                                   returned plyarray
 * ```
 *
 * This allows consecutive calls to append plies to the same buffer by passing
 * the return value of the previous call as the next call `plyarray` argument.
 *
 * @note The returned pointer must not be dereferenced.
 */
Ply* generate_pseudolegal_plies(const BoardState* board_state, Ply* plyarray);

#endif
