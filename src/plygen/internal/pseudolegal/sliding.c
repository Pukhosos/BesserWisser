#include "include/aliases/uints.h"
#include "include/bitmanip/lsb.h"
#include "include/board/enums/occupancy_kind.h"
#include "include/board/enums/piece_kind.h"
#include "include/board/state.h"
#include "include/plygen/internal/attacks.h"
#include "include/plystruct/ply.h"

#include "include/plygen/internal/pseudolegal.h"

Ply* generate_pseudolegal_sliding_plies(
    const BoardState* board_state,
    Ply* plyarray
) {
    const U16 side_to_move = board_state->ply_count & (U16)0x0001;
    const U64 occupied = board_state->bitboards[COMBINED_OCCUPANCY];
    const U64 capturable = ~board_state->bitboards[WHITE_OCCUPANCY + side_to_move];
    U64 bishops = (
        board_state->bitboards[WHITE_BISHOP + side_to_move]
        | board_state->bitboards[WHITE_QUEEN + side_to_move]
    );
    while (bishops != 0ULL) {
        U8 from = pop_lsb(&bishops);
        U64 targets = get_bishop_attacks(from, occupied) & capturable;
        while (targets != 0ULL) {
            U8 to = pop_lsb(&targets);
            *plyarray++ = make_ordinary_ply(from, to);
        }
    }
    U64 rooks = (
        board_state->bitboards[WHITE_ROOK + side_to_move]
        | board_state->bitboards[WHITE_QUEEN + side_to_move]
    );
    while (rooks != 0ULL) {
        U8 from = pop_lsb(&rooks);
        U64 targets = get_rook_attacks(from, occupied) & capturable;
        while (targets != 0ULL) {
            U8 to = pop_lsb(&targets);
            *plyarray++ = make_ordinary_ply(from, to);
        }
    }
    return plyarray;
}
