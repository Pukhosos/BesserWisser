#ifndef BOARD_STATE_H
#define BOARD_STATE_H

#include "include/aliases/uints.h"
#include "include/board/constants.h"

/**
 * @brief Represents current chess position.
 *
 * Stores the board using bitboards. The structure contains no dynamically
 * allocated memory and has a fixed size of 126 bytes.
 *
 * ```
 * typedef struct {
 *     U64 bitboards[15];
 *     U16 ply_count;
 *     U16 ply_clock;
 *     U8 castling_rights;
 *     U8 en_passant;
 * } BoardState;
 * ```
 */
typedef struct {
    /**
     * @brief Bitboards representing squares occupied by the pieces.
     *
     * Contains 15 bitboards: 12 piece-specific bitboards, two color-specific
     * occupancy bitboards, and one combined occupancy bitboard.
     *
     * @note Square indexing convention is `a1 = 0`, `b1 = 1`, `...`, `h8 = 63`.
     */
    U64 bitboards[BITBOARD_COUNT];

    /**
     * @brief Number of half-moves played.
     *
     * Side to move is inferred from parity of `ply_count`:
     *
     *   - White to move if `ply_count` is even.
     *
     *   - Black to move if `ply_count` is odd.
     *
     * Maximum value should be `MAX_PLY_COUNT = 17697`.
     */
    U16 ply_count;

    /**
     * @brief Halfmoves since the last pawn move or capture.
     *
     * Used to enforce the 50-move and 75-move rules:
     *
     *   - 50-move rule: After 100 halfmoves (50 moves by each player) without a
     *     pawn move or a capture, either player may claim a draw.
     *
     *   - 75-move rule: After 150 halfmoves (75 moves by each player) without a
     *     pawn move or a capture, the game is automatically drawn, unless the
     *     final move delivers checkmate.
     *
     * @note Resets to zero after any pawn move or any capture, otherwise
     * incremented after every move by either player.
     * */
    U16 ply_clock;

    /**
     * @brief Bitmask of `CastlingRight` values encoding the castling rights.
     *
     * ```
     * Bit:   7 6 5 4 3 2 1 0
     *        ---------------
     * Value: - - - - q Q k K
     * ```
     *
     * @note Bits 4-7 are reserved and should be zero.
     */
    U8 castling_rights;

    /**
     * @brief Square index of the current en passant target, or a sentinel value
     * `NO_EN_PASSANT = 0xFF` if no en passant target exists.
     *
     * Stores the index of the square passed over by a pawn that advanced two
     * squares from its starting rank on the previous move.
     *
     * An opposing pawn may capture onto this square on the immediately
     * following move, provided the capture does not leave its own king in
     * check.
     *
     * The target is cleared after any subsequent move unless that move
     * establishes a new en passant target.
     *
     * After:
     *
     *   - `1. e4 nc6`
     *
     * ```
     *   +------------------------+
     * 8 | r  .  b  q  k  b  n  r |
     * 7 | p  p  p  p  p  p  p  p |
     * 6 | .  .  n  .  .  .  .  . |
     * 5 | .  .  .  .  .  .  .  . |
     * 4 | .  .  .  .  P  .  .  . |
     * 3 | .  .  .  .  .  .  .  . |
     * 2 | P  P  P  P  .  P  P  P |
     * 1 | R  N  B  Q  K  B  N  R |
     *   +------------------------+
     *     a  b  c  d  e  f  g  h
     * ```
     *
     *   - `2. e5 d5`
     *
     * ```
     *   +------------------------+
     * 8 | r  .  b  q  k  b  n  r |
     * 7 | p  p  p  .  p  p  p  p |
     * 6 | .  .  n  *  .  .  .  . | <-- en_passant = d6
     * 5 | .  .  .  p  P  .  .  . |
     * 4 | .  .  .  .  .  .  .  . |
     * 3 | .  .  .  .  .  .  .  . |
     * 2 | P  P  P  P  .  P  P  P |
     * 1 | R  N  B  Q  K  B  N  R |
     *   +------------------------+
     *     a  b  c  d  e  f  g  h
     * ```
     *
     * White can capture en passant by playing `3. exd6`
     *
     * ```
     *   +------------------------+
     * 8 | r  .  b  q  k  b  n  r |
     * 7 | p  p  p  .  p  p  p  p |
     * 6 | .  .  n  P  .  .  .  . |
     * 5 | .  .  .  .  .  .  .  . |
     * 4 | .  .  .  .  .  .  .  . |
     * 3 | .  .  .  .  .  .  .  . |
     * 2 | P  P  P  P  .  P  P  P |
     * 1 | R  N  B  Q  K  B  N  R |
     *   +------------------------+
     *     a  b  c  d  e  f  g  h
     * ```
     *
     * @note When `en_passant != NO_EN_PASSANT`:
     *
     *   - The target square must be empty.
     *
     *   - The target must lie on rank 6 if white is to move, or rank 3 if black
     *     is to move.
     *
     *   - An opposing pawn must occupy the square immediately behind the
     *     target.
     */
    U8 en_passant;
} BoardState;

#endif
