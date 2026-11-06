#ifndef BITMANIP_LSB_H
#define BITMANIP_LSB_H

#include "include/aliases/uints.h"

/**
 * @brief Returns the index of the least significant set bit.
 *
 * @param bitboard Bitboard to scan.
 * @return Zero-based index of the least significant set bit.
 *
 * @note The result is undefined for a zero bitboard.
 */
inline U8 lsb(U64 bitboard) {
    return (U8)__builtin_ctzll(bitboard);
}

/**
 * @brief Resets the least significant set bit and returns its index.
 *
 * @param bitboard Pointer to the bitboard to modify.
 * @return Zero-based index of the least significant set bit.
 *
 * @note The result is undefined for a zero bitboard.
 */
inline U8 pop_lsb(U64* bitboard) {
    U8 square = lsb(*bitboard);
    *bitboard &= *bitboard - 1;
    return square;
}

#endif
