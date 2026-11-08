#ifndef BOARD_FEN_INTERFACE_H
#define BOARD_FEN_INTERFACE_H

#include "include/board/state.h"
#include "include/board/fen/enums/parsing_result.h"
#include "include/board/fen/enums/serialization_result.h"

/**
 * @brief Parse a FEN string into a board state.
 *
 * @param board_state `BoardState` to parse FEN into.
 * @param fen Null-terminated FEN string to parse.
 *
 * @return `FenParsingResult` indicating a success of a specific parsing error.
 *
 * Parse a string according to the [Forsyth–Edwards Notation specification](
 *     https://www.saremba.de/chessgml/standards/pgn/pgn-complete.htm#c16.1
 * ).
 *
 * FEN string consists of six fields:
 *
 *     1. Piece placement.
 *
 *     2. Active color.
 *
 *     3. Castling availability.
 *
 *     4. En passant target square.
 *
 *     5. Halfmove clock.
 *
 *     6. Fullmove number.
 *
 * @note On failure, `board_state` remains unchanged.
 */
FenParsingResult from_fen(BoardState* board_state, const char* fen);

/**
 * @brief Convert a board state into a FEN string.
 *
 * @param board_state `BoardState` to convert into FEN.
 * @param fen Destination buffer to store the null-terminated FEN string. The
 * buffer must have a capacity of at least `FEN_BUFFER_SIZE = 96` bytes.
 *
 * @return `FenSerializationResult` indicating a success of a specific
 * serialization error.
 *
 * Serialize the current board state into Forsyth-Edwards Notation, including
 * all six fields:
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
 * @note On failure, `fen` remains unchanged.
 */
FenSerializationResult to_fen(const BoardState* board_state, char* fen);

#endif
