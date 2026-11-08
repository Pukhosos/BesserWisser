#ifndef PLYGEN_RUNTIMECONST_MASKS_H
#define PLYGEN_RUNTIMECONST_MASKS_H

#include "include/aliases/uints.h"

/** @brief Knight attack mask at a corresponding square. */
extern const U64 KNIGHT_MASK[64];

/** @brief Bishop attack mask at a corresponding square. */
extern const U64 BISHOP_MASK[64];

/** @brief Rook attack mask at a corresponding square. */
extern const U64 ROOK_MASK[64];

/** @brief Queen attack mask at a corresponding square. */
extern const U64 QUEEN_MASK[64];

/** @brief King attack mask at a corresponding square. */
extern const U64 KING_MASK[64];

#endif
