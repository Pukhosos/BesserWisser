#include "include/board/state.h"
#include "include/plygen/internal/pseudolegal.h"
#include "include/plystruct/ply.h"

#include "include/plygen/public.h"

Ply* generate_pseudolegal_plies(const BoardState* board_state, Ply* plyarray) {
    plyarray = generate_pseudolegal_pawn_plies(board_state, plyarray);
    plyarray = generate_pseudolegal_knight_plies(board_state, plyarray);
    plyarray = generate_pseudolegal_king_plies(board_state, plyarray);
    plyarray = generate_pseudolegal_sliding_plies(board_state, plyarray);
    return plyarray;
}
