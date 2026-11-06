#ifndef PLYGEN_RUNTIMECONST_RELEVANT_MASKS_H
#define PLYGEN_RUNTIMECONST_RELEVANT_MASKS_H

#include "include/aliases/uints.h"

/** @brief Bishop relevant occupancy mask with excluded ray endpoints. */
extern const U64 BISHOP_RELEVANT_MASK[64];

/** @brief Rook relevant occupancy mask with excluded ray endpoints. */
extern const U64 ROOK_RELEVANT_MASK[64];

/** @brief Queen relevant occupancy mask with excluded ray endpoints. */
extern const U64 QUEEN_RELEVANT_MASK[64];

#endif
