// depends-on: src/board/fen/from_fen.c
// depends-on: src/board/fen/to_fen.c
// depends-on: src/board/ply.c

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/board/fen/constants.h"
#include "include/board/fen/enums/parsing_result.h"
#include "include/board/fen/enums/serialization_result.h"
#include "include/board/fen/interface.h"
#include "include/board/ply.h"
#include "include/board/state.h"
#include "include/plystruct/ply.h"

#ifndef TESTDATA
#define TESTDATA "testdata/generated/lichess-games-with-transitions.tsv"
#endif

static void test_apply_ply_matches_resulting_fen() {
    FILE* file = fopen(TESTDATA, "r");
    assert(file != NULL);
    char line[FEN_BUFFER_SIZE * 2 + 16];
    char actual_fen[FEN_BUFFER_SIZE];
    BoardState board_state;
    while (fgets(line, sizeof(line), file) != NULL) {
        // Arrange
        line[strcspn(line, "\r\n")] = '\0';
        char* fen = line;
        char* ply_text = strchr(fen, '\t');
        assert(ply_text != NULL);
        *ply_text++ = '\0';
        char* expected_fen = strchr(ply_text, '\t');
        assert(expected_fen != NULL);
        *expected_fen++ = '\0';
        assert(strchr(expected_fen, '\t') == NULL);
        char* end = NULL;
        const unsigned long parsed_ply = strtoul(ply_text, &end, 10);
        const Ply ply = (Ply)parsed_ply;
        assert(from_fen(&board_state, fen) == FEN_OK);

        // Act
        (void)apply_ply(&board_state, ply);

        // Assert
        assert(to_fen(&board_state, actual_fen) == FEN_SERIALIZATION_OK);
        assert(strcmp(actual_fen, expected_fen) == 0);
    }
    assert(fclose(file) == 0);
}

#include "tests/headers/dispatch.h"
