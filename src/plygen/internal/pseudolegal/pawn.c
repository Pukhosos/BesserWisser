#include "include/aliases/uints.h"
#include "include/bitmanip/lsb.h"
#include "include/board/constants.h"
#include "include/board/enums/square.h"
#include "include/board/enums/occupancy_kind.h"
#include "include/board/enums/piece_kind.h"
#include "include/board/state.h"
#include "include/plygen/enums/file.h"
#include "include/plygen/enums/rank.h"
#include "include/plystruct/ply.h"

#include "include/plygen/internal/pseudolegal.h"

static inline Ply* emit_pawn_plies(
    Ply* plyarray,
    U64 targets,
    U64 promotion_rank,
    U8 displacement
) {
    U64 ordinary = targets & ~promotion_rank;
    while (ordinary != 0ULL) {
        U8 to = pop_lsb(&ordinary);
        U8 from = to - displacement;
        *plyarray++ = make_ordinary_ply(from, to);
    }
    U64 promotions = targets & promotion_rank;
    while (promotions != 0ULL) {
        U8 to = pop_lsb(&promotions);
        U8 from = to - displacement;
        *plyarray++ = make_promotion_ply(from, to, KNIGHT_PROMOTION);
        *plyarray++ = make_promotion_ply(from, to, BISHOP_PROMOTION);
        *plyarray++ = make_promotion_ply(from, to, ROOK_PROMOTION);
        *plyarray++ = make_promotion_ply(from, to, QUEEN_PROMOTION);
    }
    return plyarray;
}

Ply* generate_pseudolegal_pawn_plies(
    const BoardState* board_state,
    Ply* plyarray
) {
    const U16 side_to_move = board_state->ply_count & (U16)0x0001;
    const U64 pawns = board_state->bitboards[WHITE_PAWN + side_to_move];
    const U64 empty = ~board_state->bitboards[COMBINED_OCCUPANCY];
    const U64 enemy_pieces = board_state->bitboards[BLACK_OCCUPANCY - side_to_move];
    const U8 en_passant = board_state->en_passant;
    if (side_to_move == (U16)0) {
        U64 single_push = (pawns << 8) & empty;
        U64 double_push = ((single_push & RANK_3) << 8) & empty;
        U64 capture_left = ((pawns & ~FILE_A) << 7) & enemy_pieces;
        U64 capture_right = ((pawns & ~FILE_H) << 9) & enemy_pieces;
        plyarray = emit_pawn_plies(plyarray, single_push, RANK_8, 8);
        plyarray = emit_pawn_plies(plyarray, double_push, RANK_8, 16);
        plyarray = emit_pawn_plies(plyarray, capture_left, RANK_8, 7);
        plyarray = emit_pawn_plies(plyarray, capture_right, RANK_8, 9);
        if (en_passant != NO_EN_PASSANT) {
            if (
                make_square_mask_from_square(en_passant - 9) & pawns & ~FILE_H
            ) *plyarray++ = make_en_passant_ply(en_passant - 9, en_passant);
            if (
                make_square_mask_from_square(en_passant - 7) & pawns & ~FILE_A
            ) *plyarray++ = make_en_passant_ply(en_passant - 7, en_passant);
        }
    } else {
        U64 single_push = (pawns >> 8) & empty;
        U64 double_push = ((single_push & RANK_6) >> 8) & empty;
        U64 capture_left = ((pawns & ~FILE_A) >> 9) & enemy_pieces;
        U64 capture_right = ((pawns & ~FILE_H) >> 7) & enemy_pieces;
        plyarray = emit_pawn_plies(plyarray, single_push, RANK_1, -8);
        plyarray = emit_pawn_plies(plyarray, double_push, RANK_1, -16);
        plyarray = emit_pawn_plies(plyarray, capture_left, RANK_1, -9);
        plyarray = emit_pawn_plies(plyarray, capture_right, RANK_1, -7);
        if (en_passant != NO_EN_PASSANT) {
            if (
                make_square_mask_from_square(en_passant + 7) & pawns & ~FILE_H
            ) *plyarray++ = make_en_passant_ply(en_passant + 7, en_passant);
            if (
                make_square_mask_from_square(en_passant + 9) & pawns & ~FILE_A
            ) *plyarray++ = make_en_passant_ply(en_passant + 9, en_passant);
        }
    }
    return plyarray;
}
