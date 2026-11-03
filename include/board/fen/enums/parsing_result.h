#ifndef BOARD_FEN_ENUMS_PARSING_RESULT_H
#define BOARD_FEN_ENUMS_PARSING_RESULT_H

/** @brief Result of parsing a string in Forsyth–Edwards Notation. */
typedef enum {
    /**
     * @brief FEN string parsed successfully.
     *
     * All six FEN fields are valid, and the resulting position has been
     * successfully stored in the destination `BoardState`.
     */
    FEN_OK = 0x00,

    /**
     * @brief Invalid FEN string structure.
     *
     * The FEN string must consist of exactly six non-empty fields, separated by
     * exactly one ASCII space (`0x20`), in the following order:
     *
     *   1. Piece placement.
     *
     *   2. Active color.
     *
     *   3. Castling availability.
     *
     *   4. En passant target square.
     *
     *   5. Halfmove clock.
     *
     *   6. Fullmove number.
     *
     * The string must not contain leading or trailing whitespace, consecutive
     * field separators, or whitespace characters other than the required ASCII
     * spaces.
     *
     * @note Errors specific to individual fields are reported using their
     * corresponding `FEN_INVALID_*` result.
     */
    FEN_INVALID_FORMAT = 0x01,

    /**
     * @brief Invalid piece placement field.
     *
     * The first FEN field must describe exactly eight ranks, ordered from rank
     * `8` to rank `1` and separated by exactly seven forward slashes (`/`).
     *
     * Each rank must describe exactly eight squares using:
     *
     *   - Piece characters: `PNBRQK` for White and `pnbrqk` for Black.
     *
     *   - Digits `1` through `8` representing consecutive empty squares.
     *
     * Consecutive digits are not permitted. Each rank must account for exactly
     * eight squares, and the field must contain exactly eight ranks.
     *
     * @note This result concerns piece placement notation only. It does not
     * indicate whether the resulting position is legal or reachable.
     */
    FEN_INVALID_PIECE_PLACEMENT = 0x02,

    /**
     * @brief Invalid side-to-move field.
     *
     * The second FEN field most contain exactly one of these characters:
     *
     *   - `w` for White to move.
     *
     *   - `b` for Black to move.
     */
    FEN_INVALID_SIDE_TO_MOVE = 0x03,

    /**
     * @brief Invalid castling availability field.
     *
     * The third FEN field must contain either `-` (no castling rights) or a
     * valid combination of the following characters:
     *
     *   - `K` for White kingside castling.
     *
     *   - `Q` for White queenside castling.
     *
     *   - `k` for Black kingside castling.
     *
     *   - `q` for Black queenside castling.
     *
     * Castling availability characters must appear at most once and in the
     * specified order: `KQkq`. Any subsequence of this ordering is permitted.
     *
     * @note This result concerns the field notation only. The presence of a
     * castling right does not imply that the corresponding king and rook are
     * currently in their original positions or that castling is legal.
     */
    FEN_INVALID_CASTLING_AVAILABILITY = 0x04,

    /**
     * @brief Invalid en passant target square field.
     *
     * The fourth FEN field must contain either `-` (no en passant target) or a
     * valid target square in algebraic notation.
     *
     * The target square must:
     *
     *   - Have a file between `a` and `h`.
     *
     *   - Be on rank `3` when Black is to move.
     *
     *   - Be on rank `6` when White is to move.
     *
     *   - The target square must be unoccupied.
     *
     *   - The pawn that advanced two squares must occupy the square immediately
     *     beyond the target square in the direction of its movement.
     *
     *   - The pawn's original square must be unoccupied.
     *
     * @note A valid en passant target does not necessarily imply that an en
     * passant capture is currently legal. According to the
     * FEN specification, the target square must be recorded after a double
     * pawn push, even if the opponent has no pawn capable of capturing en
     * passant.
     *
     * For example, after `1. e2-e4`, the en passant target is `e3`, regardless
     * of whether Black has a pawn on `d4` or `f4` or not.
     *
     * ```
     *   +------------------------+
     * 8 | r  n  b  q  k  b  n  r |
     * 7 | p  p  p  p  p  p  p  p |
     * 6 | .  .  .  .  .  .  .  . |
     * 5 | .  .  .  .  .  .  .  . |
     * 4 | .  .  .  _  P  _  .  . |
     * 3 | .  .  .  .  *  .  .  . | <-- en_passant = e3
     * 2 | P  P  P  P  .  P  P  P |
     * 1 | R  N  B  Q  K  B  N  R |
     *   +------------------------+
     *     a  b  c  d  e  f  g  h
     * ```
     *
     * Only black pawns on `d4` and `f4` could potentially capture the pawn on
     * `e4` en passant. Even though neither pawn is present in this position,
     * the FEN must still include `e3` as the en passant target square.
     */
    FEN_INVALID_EN_PASSANT = 0x05,

    /**
     * @brief Invalid halfmove clock field.
     *
     * The fifth FEN field must contain a non-negative decimal integer
     * representing the number of halfmoves since the last pawn move or capture.
     *
     * @note The upper bound of `UINT16_MAX = 65535` is an implementation
     * limitation, not a FEN specification requirement. Although halfmove clock
     * shall never exceed 150 in a valid chess game anyway due to 75-move rule.
     *
     * @see [FIDE Laws of Chess (2023) — Article 9.6.2](
     *     https://handbook.fide.com/chapter/e012023#:~:text=9%2E6%2E2,-any
     * ).
     */
    FEN_INVALID_HALFMOVE_CLOCK = 0x06,

    /**
     * @brief Invalid fullmove number field.
     *
     * The sixth FEN field must contain a positive decimal integer representing
     * the current fullmove number. The fullmove number starts at 1 and
     * increments after each move by Black.
     *
     * @note The upper bound of `UINT16_MAX = 65535` is an implementation
     * limitation, not a FEN specification requirement. Although fullmove number
     * shall never exceed 8849 in a valid chess game anyway due to 75-move rule.
     *
     * @see [arXiv:2608.14762 [math.GM]](https://arxiv.org/abs/2608.14762).
     */
    FEN_INVALID_FULLMOVE_NUMBER = 0x07,

    /** @brief `board_state` is NULL. */
    FEN_NULL_BOARD_STATE = 0x08,

    /**
     * @brief `fen` is NULL.
     *
     * The source FEN string pointer is NULL. No parsing is performed.
     *
     * @note The destination `board_state` remains unchanged.
     */
    FEN_NULL_FEN = 0x09,
} FenParsingResult;

#endif
