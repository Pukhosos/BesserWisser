// depends-on: src/bitmanip/lsb.c
// depends-on: src/board/from_fen.c
// depends-on: src/plygen/internal/attacks/pext/bishop.c
// depends-on: src/plygen/internal/attacks/pext/rook.c
// depends-on: src/plygen/internal/attacks/pext/square.c
// depends-on: src/plygen/internal/pseudolegal/king.c
// depends-on: src/plygen/internal/pseudolegal/knight.c
// depends-on: src/plygen/internal/pseudolegal/pawn.c
// depends-on: src/plygen/internal/pseudolegal/sliding.c
// depends-on: src/plygen/public/pseudolegal.c
// depends-on: src/plygen/runtimeconst/attack_tables/pext/bishop.c
// depends-on: src/plygen/runtimeconst/attack_tables/pext/rook.c
// depends-on: src/plygen/runtimeconst/masks.c
// depends-on: src/plygen/runtimeconst/relevant_masks.c

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/aliases/uints.h"
#include "include/board/state.h"
#include "include/board/fen/enums/parsing_result.h"
#include "include/board/fen/interface.h"
#include "include/plygen/public.h"
#include "include/plystruct/ply.h"

#ifndef PLYARRAY_SIZE
#define PLYARRAY_SIZE 256
#endif

#ifndef FEN_TESTDATA
#define FEN_TESTDATA "testdata/generated/lichess-games-pseudolegal-moves.fen"
#endif

static void test_pseudolegal_plies_from_fen() {
    FILE* file = fopen(FEN_TESTDATA, "r");
    assert(file != NULL);
    char line[256];
    BoardState board_state;
    static Ply plyarray[PLYARRAY_SIZE];
    while (fgets(line, sizeof(line), file) != NULL) {
        // Arrange
        char* separator = strchr(line, '\t');
        assert(separator != NULL);
        *separator = '\0';
        U16 expected_ply_count = (U16)strtoul(separator + 1, NULL, 10);
        FenParsingResult result = from_fen(&board_state, line);
        assert(result == FEN_OK);

        // Act
        Ply* last_ply = generate_pseudolegal_plies(&board_state, plyarray);

        // Assert
        U16 actual_ply_count = (U16)(last_ply - plyarray);
        if (actual_ply_count != expected_ply_count) {
            fprintf(
                stderr,
                "Move count mismatch!\n"
                "FEN:      %s\n"
                "Expected: %u\n"
                "Actual:   %u\n"
                "Plies:      \n",
                line,
                (U32)expected_ply_count,
                (U32)actual_ply_count
            );
            char uci[6];
            for (Ply* ply = plyarray; ply < last_ply; ++ply) {
                to_uci_notation(uci, *ply);
                fprintf(stderr, "\t%s\n", uci);
            }
        }
        assert(actual_ply_count == expected_ply_count);
    }
    assert(fclose(file) == 0);
}

#include "tests/headers/dispatch.h"
