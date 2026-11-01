#ifndef BOARD_ENUMS_SQUARE_H
#define BOARD_ENUMS_SQUARE_H

#include "include/aliases/uints.h"

/**
 * @brief Chessboard square indices using Little-Endian Rank-File (LERF)
 * mapping.
 *
 * Squares are indexed from `0` (A1) to `63` (H8), with files increasing from A
 * to H and ranks increasing from 1 to 8.
 *
 * The file and rank of a square can be extracted using:
 *
 * ```c
 * U8 file = square & 7;    // 0 = A, 7 = H
 * U8 rank = square >> 3;   // 0 = rank 1, 7 = rank 8
 * ```
 */
typedef enum : U8 {
    A1 = 0b00000000,
    B1 = 0b00000001,
    C1 = 0b00000010,
    D1 = 0b00000011,
    E1 = 0b00000100,
    F1 = 0b00000101,
    G1 = 0b00000110,
    H1 = 0b00000111,

    A2 = 0b00001000,
    B2 = 0b00001001,
    C2 = 0b00001010,
    D2 = 0b00001011,
    E2 = 0b00001100,
    F2 = 0b00001101,
    G2 = 0b00001110,
    H2 = 0b00001111,

    A3 = 0b00010000,
    B3 = 0b00010001,
    C3 = 0b00010010,
    D3 = 0b00010011,
    E3 = 0b00010100,
    F3 = 0b00010101,
    G3 = 0b00010110,
    H3 = 0b00010111,

    A4 = 0b00011000,
    B4 = 0b00011001,
    C4 = 0b00011010,
    D4 = 0b00011011,
    E4 = 0b00011100,
    F4 = 0b00011101,
    G4 = 0b00011110,
    H4 = 0b00011111,

    A5 = 0b00100000,
    B5 = 0b00100001,
    C5 = 0b00100010,
    D5 = 0b00100011,
    E5 = 0b00100100,
    F5 = 0b00100101,
    G5 = 0b00100110,
    H5 = 0b00100111,

    A6 = 0b00101000,
    B6 = 0b00101001,
    C6 = 0b00101010,
    D6 = 0b00101011,
    E6 = 0b00101100,
    F6 = 0b00101101,
    G6 = 0b00101110,
    H6 = 0b00101111,

    A7 = 0b00110000,
    B7 = 0b00110001,
    C7 = 0b00110010,
    D7 = 0b00110011,
    E7 = 0b00110100,
    F7 = 0b00110101,
    G7 = 0b00110110,
    H7 = 0b00110111,

    A8 = 0b00111000,
    B8 = 0b00111001,
    C8 = 0b00111010,
    D8 = 0b00111011,
    E8 = 0b00111100,
    F8 = 0b00111101,
    G8 = 0b00111110,
    H8 = 0b00111111,
} Square;

#endif
