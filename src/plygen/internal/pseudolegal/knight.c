#include "include/aliases/uints.h"
#include "include/bitmanip/lsb.h"
#include "include/board/enums/occupancy_kind.h"
#include "include/board/enums/piece_kind.h"
#include "include/board/state.h"
#include "include/plygen/runtimeconst/masks.h"
#include "include/plystruct/ply.h"

#include "include/plygen/internal/pseudolegal.h"

Ply* generate_pseudolegal_knight_plies(
    const BoardState* board_state,
    Ply* plyarray
) {
    const U16 side_to_move = board_state->ply_count & (U16)0x0001;
    const U64 own_pieces = board_state->bitboards[WHITE_OCCUPANCY + side_to_move];
    U64 knights = board_state->bitboards[WHITE_KNIGHT + side_to_move];
    while (knights != 0ULL) {
        U8 from = pop_lsb(&knights);
        U64 targets = KNIGHT_MASK[from] & ~own_pieces;
        while (targets != 0ULL) {
            U8 to = pop_lsb(&targets);
            *plyarray++ = make_ordinary_ply(from, to);
        }
    }
    return plyarray;
}
