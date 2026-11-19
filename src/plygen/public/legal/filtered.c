#include "include/aliases/uints.h"
#include "include/board/enums/piece_kind.h"
#include "include/board/enums/square.h"
#include "include/board/ply.h"
#include "include/board/state.h"
#include "include/plystruct/ply.h"

#include "include/plygen/public.h"

static inline bool is_ply_valid(
    const BoardState* board_state,
    Ply* plyarray,
    const Ply ply
) {
    bool result = true;
    const Unply unply = apply_ply((BoardState*)board_state, ply);
    const U16 side_to_move = board_state->ply_count & (U16)0x0001;
    Square king_square = make_square_from_square_mask(
        board_state->bitboards[BLACK_KING - side_to_move]
    );
    Ply* opponents_end = generate_pseudolegal_plies(board_state, plyarray);
    for (
        Ply* opponents_ply = plyarray;
        opponents_ply < opponents_end;
        ++opponents_ply
    ) if (get_ply_to_square(*opponents_ply) == king_square) {
        result = false;
        break;
    }
    undo_ply((BoardState*)board_state, ply, unply);
    return result;
}

Ply* generate_legal_plies(const BoardState* board_state, Ply* plyarray) {
    Ply* pseudolegal_end = generate_pseudolegal_plies(board_state, plyarray);
    Ply* legal_end = plyarray;
    for (Ply* ply = plyarray; ply < pseudolegal_end; ++ply) {
        if (is_ply_valid(board_state, pseudolegal_end, *ply)) {
            *legal_end = *ply;
            *legal_end++;
        }
    }
    return legal_end;
}
