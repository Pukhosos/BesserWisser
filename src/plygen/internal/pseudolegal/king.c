#include "include/aliases/uints.h"
#include "include/bitmanip/lsb.h"
#include "include/board/enums/castling_right.h"
#include "include/board/enums/occupancy_kind.h"
#include "include/board/enums/piece_kind.h"
#include "include/board/enums/square.h"
#include "include/board/state.h"
#include "include/plygen/internal/attacks.h"
#include "include/plygen/runtimeconst/masks.h"
#include "include/plystruct/ply.h"

#include "include/plygen/internal/pseudolegal.h"

Ply* generate_pseudolegal_king_plies(
    const BoardState* board_state,
    Ply* plyarray
) {
    const U16 side_to_move = board_state->ply_count & (U16)0x0001;
    const U64 own_pieces = board_state->bitboards[WHITE_OCCUPANCY + side_to_move];
    const U64 occupied = board_state->bitboards[COMBINED_OCCUPANCY];
    const U64 enemy_attacks = get_attacked_squares(board_state, side_to_move ^ (U16)1);
    const Square king_square = make_square_from_square_mask(
        board_state->bitboards[WHITE_KING + side_to_move]
    );
    const U8 castling_rights = board_state->castling_rights;
    U64 targets = KING_MASK[king_square] & ~own_pieces;
    while (targets != 0ULL) {
        U8 to = pop_lsb(&targets);
        *plyarray++ = make_ordinary_ply(king_square, to);
    }
    if (side_to_move == (U16)0) {
        if (
            castling_rights & CAN_WHITE_CASTLE_SHORT
            && (enemy_attacks & (E1_MASK | F1_MASK | G1_MASK)) == 0ULL
            && (occupied & (F1_MASK | G1_MASK)) == 0ULL
        ) *plyarray++ = make_castling_ply(E1, G1);
        if (
            castling_rights & CAN_WHITE_CASTLE_LONG
            && (enemy_attacks & (C1_MASK | D1_MASK | E1_MASK)) == 0ULL
            && (occupied & (B1_MASK | C1_MASK | D1_MASK)) == 0ULL
        ) *plyarray++ = make_castling_ply(E1, C1);
    } else {
        if (
            castling_rights & CAN_BLACK_CASTLE_SHORT
            && (enemy_attacks & (E8_MASK | F8_MASK | G8_MASK)) == 0ULL
            && (occupied & (F8_MASK | G8_MASK)) == 0ULL
        ) *plyarray++ = make_castling_ply(E8, G8);
        if (
            castling_rights & CAN_BLACK_CASTLE_LONG
            && (enemy_attacks & (C8_MASK | D8_MASK | E8_MASK)) == 0ULL
            && (occupied & (B8_MASK | C8_MASK | D8_MASK)) == 0ULL
        ) *plyarray++ = make_castling_ply(E8, C8);
    }
    return plyarray;
}
