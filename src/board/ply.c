#include "include/aliases/uints.h"
#include "include/board/ply.h"
#include "include/board/constants.h"
#include "include/board/state.h"
#include "include/board/enums/castling_right.h"
#include "include/board/enums/occupancy_kind.h"
#include "include/board/enums/piece_kind.h"
#include "include/board/enums/square.h"
#include "include/plystruct/enums/ply_kind.h"
#include "include/plystruct/enums/promotion_kind.h"
#include "include/plystruct/ply.h"
#include "include/plystruct/unply.h"

static const U8 castling_rights_lost[64] = {
    [A1] = CAN_WHITE_CASTLE_LONG,
    [E1] = CAN_WHITE_CASTLE_SHORT | CAN_WHITE_CASTLE_LONG,
    [H1] = CAN_WHITE_CASTLE_SHORT,
    [A8] = CAN_BLACK_CASTLE_LONG,
    [E8] = CAN_BLACK_CASTLE_SHORT | CAN_BLACK_CASTLE_LONG,
    [H8] = CAN_BLACK_CASTLE_SHORT,
};

static inline U8 get_piece_kind_on_square(
    const BoardState* board_state,
    U16 side_to_move,
    SquareMask square_mask
) {
    for (
        PieceKind piece = (PieceKind)side_to_move;
        piece <= BLACK_KING;
        piece += 2
    ) if (board_state->bitboards[piece] & square_mask) return piece;
    return EMPTY_SQUARE;
}

static inline PieceKind get_moving_piece_kind(
    const BoardState* board_state,
    U16 side_to_move,
    PlyKind ply_kind,
    SquareMask square_mask
) {
    switch (ply_kind) {
        case PROMOTION_MOVE:    /* fallthrough */
        case EN_PASSANT_MOVE:   return WHITE_PAWN + side_to_move;
        case CASTLING_MOVE:     return WHITE_KING + side_to_move;
        case ORDINARY_MOVE:     return (PieceKind)get_piece_kind_on_square(
            board_state,
            side_to_move,
            square_mask
        );
        default: __builtin_unreachable();
    }
}

static inline PieceKind get_promotion_piece_kind(
    PromotionKind promotion_kind,
    U16 side_to_move
) {
    switch (promotion_kind) {
        case KNIGHT_PROMOTION:  return (PieceKind)(WHITE_KNIGHT + side_to_move);
        case BISHOP_PROMOTION:  return (PieceKind)(WHITE_BISHOP + side_to_move);
        case ROOK_PROMOTION:    return (PieceKind)(WHITE_ROOK   + side_to_move);
        case QUEEN_PROMOTION:   return (PieceKind)(WHITE_QUEEN  + side_to_move);
        default: __builtin_unreachable();
    }
}

static inline U64 get_castling_rook_delta(Square king_destination_square) {
    switch (king_destination_square) {
        case G1: return H1_MASK | F1_MASK;
        case C1: return A1_MASK | D1_MASK;
        case G8: return H8_MASK | F8_MASK;
        case C8: return A8_MASK | D8_MASK;
        default: __builtin_unreachable();
    }
}

static inline U8 apply_en_passant_ply(
    BoardState* board_state,
    U16 side_to_move,
    Square to,
    SquareMask from_mask,
    SquareMask to_mask
) {
    const Square captured_pawn_square = (Square)(
        side_to_move == (U16)0
        ? to - (U16)8
        : to + (U16)8
    );
    const SquareMask captured_mask = make_square_mask_from_square(captured_pawn_square);
    const U64 ply_delta = from_mask | to_mask;
    board_state->bitboards[WHITE_PAWN + side_to_move] ^= ply_delta;
    board_state->bitboards[WHITE_OCCUPANCY + side_to_move] ^= ply_delta;
    board_state->bitboards[BLACK_PAWN - side_to_move] ^= captured_mask;
    board_state->bitboards[BLACK_OCCUPANCY - side_to_move] ^= captured_mask;
    return BLACK_PAWN - side_to_move;
}

static inline U8 apply_promotion_ply(
    BoardState* board_state,
    U16 side_to_move,
    PromotionKind promotion_kind,
    SquareMask from_mask,
    SquareMask to_mask
) {
    const PieceKind promotion_piece_kind = get_promotion_piece_kind(
        promotion_kind,
        side_to_move
    );
    const U64 ply_delta = from_mask | to_mask;
    const U8 captured_piece_kind = get_piece_kind_on_square(
        board_state,
        (U16)1 - side_to_move,
        to_mask
    );
    board_state->bitboards[WHITE_PAWN + side_to_move] ^= from_mask;
    board_state->bitboards[promotion_piece_kind] |= to_mask;
    board_state->bitboards[WHITE_OCCUPANCY + side_to_move] ^= ply_delta;
    if (captured_piece_kind != EMPTY_SQUARE) {
        board_state->bitboards[captured_piece_kind] &= ~to_mask;
        board_state->bitboards[BLACK_OCCUPANCY - side_to_move] &= ~to_mask;
    }
    return captured_piece_kind;
}

static inline U8 apply_castling_ply(
    BoardState* board_state,
    U16 side_to_move,
    Square to,
    SquareMask from_mask,
    SquareMask to_mask
) {
    const U64 ply_delta = from_mask | to_mask;
    const U64 rook_delta = get_castling_rook_delta(to);
    board_state->bitboards[WHITE_ROOK + side_to_move] ^= rook_delta;
    board_state->bitboards[WHITE_KING + side_to_move] ^= ply_delta;
    board_state->bitboards[WHITE_OCCUPANCY + side_to_move] ^= ply_delta | rook_delta;
    return EMPTY_SQUARE;
}

static inline U8 apply_ordinary_ply(
    BoardState* board_state,
    U16 side_to_move,
    PieceKind moving_piece_kind,
    SquareMask from_mask,
    SquareMask to_mask
) {
    const U8 captured_piece_kind = get_piece_kind_on_square(
        board_state,
        (U16)1 - side_to_move,
        to_mask
    );
    const U64 ply_delta = from_mask | to_mask;
    board_state->bitboards[moving_piece_kind] ^= ply_delta;
    board_state->bitboards[WHITE_OCCUPANCY + side_to_move] ^= ply_delta;
    if (captured_piece_kind != EMPTY_SQUARE) {
        board_state->bitboards[captured_piece_kind] &= ~to_mask;
        board_state->bitboards[BLACK_OCCUPANCY - side_to_move] &= ~to_mask;
    }
    return captured_piece_kind;
}

static inline U8 get_captured_piece_kind(
    BoardState* board_state,
    PlyKind ply_kind,
    PromotionKind promotion_kind,
    U16 side_to_move,
    Square to,
    PieceKind moving_piece_kind,
    SquareMask from_mask,
    SquareMask to_mask
) {
    switch (ply_kind) {
        case EN_PASSANT_MOVE:   return apply_en_passant_ply(
            board_state,
            side_to_move,
            to,
            from_mask,
            to_mask
        );
        case PROMOTION_MOVE:    return apply_promotion_ply(
            board_state,
            side_to_move,
            promotion_kind,
            from_mask,
            to_mask
        );
        case CASTLING_MOVE:     return apply_castling_ply(
            board_state,
            side_to_move,
            to,
            from_mask,
            to_mask
        );
        case ORDINARY_MOVE:     return apply_ordinary_ply(
            board_state,
            side_to_move,
            moving_piece_kind,
            from_mask,
            to_mask
        );
        default: __builtin_unreachable();
    }
}

Unply apply_ply(BoardState* board_state, Ply ply) {
    const PlyKind ply_kind = get_ply_kind(ply);
    const U16 side_to_move = board_state->ply_count & (U16)0x0001;
    const PromotionKind promotion_kind = get_ply_promotion_kind(ply);
    const Square from = get_ply_from_square(ply);
    const Square to = get_ply_to_square(ply);
    const SquareMask from_mask = make_square_mask_from_square(from);
    const SquareMask to_mask = make_square_mask_from_square(to);
    const PieceKind moving_piece_kind = get_moving_piece_kind(
        board_state,
        side_to_move,
        ply_kind,
        from_mask
    );
    const U8 captured_piece_kind = get_captured_piece_kind(
        board_state,
        ply_kind,
        promotion_kind,
        side_to_move,
        to,
        moving_piece_kind,
        from_mask,
        to_mask
    );
    Unply unply = make_unply(
        board_state->castling_rights,
        captured_piece_kind,
        board_state->en_passant,
        board_state->ply_clock
    );
    board_state->bitboards[COMBINED_OCCUPANCY] = (
        board_state->bitboards[WHITE_OCCUPANCY]
        | board_state->bitboards[BLACK_OCCUPANCY]
    );
    board_state->castling_rights &= ~(
        castling_rights_lost[from]
        | castling_rights_lost[to]
    );
    board_state->en_passant = (
        (
            moving_piece_kind == WHITE_PAWN + side_to_move
            && (from ^ to) == 16
        )
        ? (U8)((from + to) >> 1)
        : NO_EN_PASSANT
    );
    board_state->ply_clock = (U16)(
        (
            moving_piece_kind == WHITE_PAWN + side_to_move
            || captured_piece_kind != EMPTY_SQUARE
        )
        ? (U16)0
        : board_state->ply_clock + 1
    );
    ++board_state->ply_count;
    return unply;
}
