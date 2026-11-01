#ifndef BOARD_FEN_INTERFACE_H
#define BOARD_FEN_INTERFACE_H

#include "include/board/state.h"
#include "include/board/fen/enums/parsing_result.h"

/**
 * @brief Parse a FEN string into a chess position.
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

#endif
