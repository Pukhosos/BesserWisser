#include "include/aliases/uints.h"
#include "include/bitmanip/lsb.h"
#include "include/board/enums/occupancy_kind.h"
#include "include/board/enums/piece_kind.h"
#include "include/board/state.h"
#include "include/plygen/enums/file.h"
#include "include/plygen/runtimeconst/masks.h"

#include "include/plygen/internal/attacks.h"

U64 get_attacked_squares(const BoardState* board_state, U16 attacking_side) {
    const U64 occupied = board_state->bitboards[COMBINED_OCCUPANCY];
    U64 attacks = 0ULL;
    U64 pawns = board_state->bitboards[WHITE_PAWN + attacking_side];
    if (attacking_side == (U16)0) {
        attacks |= ((pawns & ~FILE_A) << 7) | ((pawns & ~FILE_H) << 9);
    } else {
        attacks |= ((pawns & ~FILE_A) >> 9) | ((pawns & ~FILE_H) >> 7);
    }
    U64 knights = board_state->bitboards[WHITE_KNIGHT + attacking_side];
    while (knights != 0ULL) {
        U8 from = pop_lsb(&knights);
        attacks |= KNIGHT_MASK[from];
    }
    U64 kings = board_state->bitboards[WHITE_KING + attacking_side];
    while (kings != 0ULL) {
        U8 from = pop_lsb(&kings);
        attacks |= KING_MASK[from];
    }
    U64 bishops = (
        board_state->bitboards[WHITE_BISHOP + attacking_side]
        | board_state->bitboards[WHITE_QUEEN + attacking_side]
    );
    while (bishops != 0ULL) {
        U8 from = pop_lsb(&bishops);
        attacks |= get_bishop_attacks(from, occupied);
    }
    U64 rooks = (
        board_state->bitboards[WHITE_ROOK + attacking_side]
        | board_state->bitboards[WHITE_QUEEN + attacking_side]
    );
    while (rooks != 0ULL) {
        U8 from = pop_lsb(&rooks);
        attacks |= get_rook_attacks(from, occupied);
    }
    return attacks;
}
