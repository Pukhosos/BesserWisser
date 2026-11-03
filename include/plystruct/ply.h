#ifndef PLYSTRUCT_PLY_H
#define PLYSTRUCT_PLY_H

#include "include/aliases/uints.h"

/**
 * @brief 16-bit encoded chess ply.
 *
 * Ply information is packed into a 16-bit integer in the following way:
 *
 * ```
 * Bit:   F E | D C | B A 9 8 7 6 | 5 4 3 2 1 0
 *        -------------------------------------
 * Value: k k | p p | f f f f f f | t t t t t t
 * ```
 *
 * where:
 *
 *     1. Bits 0-5 encode `to` square index (bits `t`).
 *
 *     2. Bits 6-B encode `from` square index (bits `f`).
 *
 *     3. Bits C-D encode a promotion piece (bits `p`).
 *
 *     4. Bits E-F encode ply kind (bits `k`).
 */
typedef U16 Ply;

#endif
