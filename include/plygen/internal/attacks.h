#ifndef PLYGEN_INTERNAL_ATTACKS_H
#define PLYGEN_INTERNAL_ATTACKS_H

#include "include/aliases/uints.h"
#include "include/board/enums/square.h"
#include "include/board/state.h"

/**
 * @brief Compute the bishop attack bitboard for a given square and occupancy.
 *
 * @param square Index of a square occupied by the bishop.
 * @param occupancy Bitboard representing all occupied squares.
 *
 * @return Bitboard containing all squares attacked by the bishop.
 *
 * @note Attack rays stop at the first occupied square in each diagonal
 * direction including that square.
 */
U64 get_bishop_attacks(Square square, U64 occupancy);

#endif
