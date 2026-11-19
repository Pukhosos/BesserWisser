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
// depends-on: src/plygen/public/pseudolegal.c
// depends-on: src/plygen/runtimeconst/attack_tables/pext/bishop.c
// depends-on: src/plygen/runtimeconst/attack_tables/pext/rook.c
// depends-on: src/plygen/runtimeconst/masks.c
// depends-on: src/plygen/runtimeconst/relevant_masks.c

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "include/board/fen/constants.h"
#include "include/board/fen/enums/parsing_result.h"
#include "include/board/fen/interface.h"
#include "include/board/ply.h"
#include "include/board/state.h"
#include "include/plygen/public.h"
#include "include/plystruct/ply.h"
#include "include/plystruct/unply.h"

#ifndef PLYARRAY_SIZE
#define PLYARRAY_SIZE 256
#endif

#ifndef TESTDATA
#define TESTDATA "testdata/generated/lichess-games.fen"
#endif

static void test_undo_ply_inverts_apply_ply() {
    FILE* file = fopen(TESTDATA, "r");
    assert(file != NULL);
    char fen[FEN_BUFFER_SIZE];
    BoardState board_state;
    BoardState original_board_state;
    static Ply plyarray[PLYARRAY_SIZE];
    while (fgets(fen, sizeof(fen), file) != NULL) {
        fen[strcspn(fen, "\r\n")] = '\0';
        const FenParsingResult from_result = from_fen(&board_state, fen);
        assert(from_result == FEN_OK);
        memcpy(&original_board_state, &board_state, sizeof(BoardState));
        Ply* ply_end = generate_pseudolegal_plies(&board_state, plyarray);
        for (Ply* ply = plyarray; ply < ply_end; ++ply) {
            const Unply unply = apply_ply(&board_state, *ply);
            undo_ply(&board_state, *ply, unply);
            assert(
                memcmp(
                    &original_board_state,
                    &board_state,
                    sizeof(BoardState)
                ) == 0
            );
        }
    }
    assert(fclose(file) == 0);
}

#include "tests/headers/dispatch.h"
