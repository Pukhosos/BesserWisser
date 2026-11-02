// depends-on: src/board/from_fen.c
// depends-on: src/board/to_fen.c

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "include/board/fen/constants.h"
#include "include/board/fen/enums/parsing_result.h"
#include "include/board/fen/enums/serialization_result.h"
#include "include/board/fen/interface.h"
#include "include/board/state.h"

#ifndef FEN_TESTDATA
#define FEN_TESTDATA "testdata/generated/lichess-games.fen"
#endif

static void test_to_fen_inverts_from_fen() {
    FILE *file = fopen(FEN_TESTDATA, "r");
    assert(file != NULL);
    char original_fen[FEN_BUFFER_SIZE];
    char serialized_fen[FEN_BUFFER_SIZE];
    BoardState board_state;
    while (fgets(original_fen, sizeof(original_fen), file) != NULL) {
        original_fen[strcspn(original_fen, "\r\n")] = '\0';
        FenParsingResult from_result = from_fen(&board_state, original_fen);
        assert(from_result == FEN_OK);
        FenSerializationResult to_result = to_fen(&board_state, serialized_fen);
        assert(to_result == FEN_SERIALIZATION_OK);
        assert(strcmp(original_fen, serialized_fen) == 0);
    }
    assert(fclose(file) == 0);
}

#include "tests/headers/dispatch.h"
