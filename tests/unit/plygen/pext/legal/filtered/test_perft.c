// depends-on: src/bitmanip/lsb.c
// depends-on: src/board/fen/from_fen.c
// depends-on: src/board/ply.c
// depends-on: src/plygen/internal/attacks/pext/bishop.c
// depends-on: src/plygen/internal/attacks/pext/rook.c
// depends-on: src/plygen/internal/attacks/pext/square.c
// depends-on: src/plygen/internal/pseudolegal/king.c
// depends-on: src/plygen/internal/pseudolegal/knight.c
// depends-on: src/plygen/internal/pseudolegal/pawn.c
// depends-on: src/plygen/internal/pseudolegal/sliding.c
// depends-on: src/plygen/public/legal/filtered.c
// depends-on: src/plygen/public/pseudolegal.c
// depends-on: src/plygen/runtimeconst/attack_tables/pext/bishop.c
// depends-on: src/plygen/runtimeconst/attack_tables/pext/rook.c
// depends-on: src/plygen/runtimeconst/masks.c
// depends-on: src/plygen/runtimeconst/relevant_masks.c

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/board/fen/enums/parsing_result.h"
#include "include/board/fen/interface.h"
#include "include/board/ply.h"
#include "include/board/state.h"
#include "include/plygen/public.h"
#include "include/plystruct/ply.h"
#include "include/plystruct/unply.h"

#ifndef TESTDATA
#define TESTDATA "testdata/manual/perft.tsv"
#endif

#ifndef MAX_DEPTH
#define MAX_DEPTH 4
#endif

#ifndef PLYARRAY_SIZE
#define PLYARRAY_SIZE 4096
#endif

#ifndef TESTDATA_LINE_BUFFER_SIZE
#define TESTDATA_LINE_BUFFER_SIZE 1024
#endif

/**
 * @brief Calculate the number of nodes at a given depth.
 *
 * @param board_state Current board state.
 * @param depth Remaining search depth.
 *
 * @return Number of nodes at the requested depth.
 */
static U64 perft(BoardState* board_state, U32 depth) {
    if (depth == 0) return (U64)1;
    Ply plyarray[PLYARRAY_SIZE];
    Ply* const ply_end = generate_legal_plies(board_state, plyarray);
    U64 nodes = 0;
    for (Ply* ply = plyarray; ply != ply_end; ++ply) {
        const Ply candidate = *ply;
        const Unply unply = apply_ply(board_state, candidate);
        nodes += perft(board_state, depth - 1);
        undo_ply(board_state, candidate, unply);
    }
    return nodes;
}

// mark: perft
static void test_perft_matches_reference_counts() {
    FILE* file = fopen(TESTDATA, "r");
    assert(file != NULL);
    char line[TESTDATA_LINE_BUFFER_SIZE];
    BoardState board_state;
    while (fgets(line, sizeof(line), file) != NULL) {
        assert(strchr(line, '\n') != NULL || feof(file));
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0') continue;
        char* expected_field = strchr(line, '\t');
        assert(expected_field != NULL);
        *expected_field++ = '\0';
        assert(from_fen(&board_state, line) == FEN_OK);
        U32 depth = 1;
        while (expected_field != NULL && depth <= MAX_DEPTH) {
            // Arrange
            char* next_field = strchr(expected_field, '\t');
            if (next_field != NULL) {
                *next_field = '\0';
                ++next_field;
            }
            const U64 expected_nodes = strtoull(expected_field, NULL, 10);

            // Act
            const U64 actual_nodes = perft(&board_state, depth);

            // Assert
            assert(actual_nodes == expected_nodes);

            expected_field = next_field;
            ++depth;
        }
    }
    assert(fclose(file) == 0);
}

#include "tests/headers/dispatch.h"
