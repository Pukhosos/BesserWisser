#ifndef PLYSTRUCT_PLY_H
#define PLYSTRUCT_PLY_H

#include "include/aliases/uints.h"
#include "include/plystruct/enums/ply_kind.h"
#include "include/plystruct/enums/promotion_kind.h"
#include "include/plystruct/enums/masks.h"

/**
 * @brief 16-bit encoded chess ply.
 *
 * Ply information is packed into a 16-bit integer in the following way:
 *
 * ```
 * Bit:   F E | D C | B A 9 8 7 6 | 5 4 3 2 1 0
 *        -------------------------------------
 * Value: k k | p p | f f f f f f | t t t t t t
 * ```
 *
 * where:
 *
 *     1. Bits 0-5 encode `to` square index (bits `t`).
 *
 *     2. Bits 6-B encode `from` square index (bits `f`).
 *
 *     3. Bits C-D encode a promotion piece (bits `p`).
 *
 *     4. Bits E-F encode ply kind (bits `k`).
 */
typedef U16 Ply;

/**
 * @brief Make a 16-bit encoded chess ply.
 *
 * @param from Source square (0–63).
 * @param to Destination square (0–63).
 * @param ply_kind Kind of ply (ORDINARY_MOVE, PROMOTION_MOVE, EN_PASSANT_MOVE,
 * or CASTLING_MOVE).
 * @param promotion Piece to promote to.
 *
 * @return 16-bit encoded chess ply.
 *
 * ```
 * Bit:   F E | D C | B A 9 8 7 6 | 5 4 3 2 1 0
 *        -------------------------------------
 * Value: k k | p p | f f f f f f | t t t t t t
 * ```
 *
 * where:
 *
 *     1. Bits 0-5 encode `to` square index (bits `t`).
 *
 *     2. Bits 6-B encode `from` square index (bits `f`).
 *
 *     3. Bits C-D encode a promotion piece (bits `p`).
 *
 *     4. Bits E-F encode ply kind (bits `k`).
 * 
 * @note `promotion` is relevant iff `ply_kind` is `PROMOTION_MOVE`.
 */
static inline Ply make_ply(
    U8 from,
    U8 to,
    PlyKind ply_kind,
    PromotionKind promotion
) {
    return ply_kind | promotion | (U16)from << 6 | (U16)to;
}

/**
 * @brief Make an ordinary chess ply.
 *
 * @param from Source square index (0–63).
 * @param to Destination square index (0–63).
 *
 * @return 16-bit encoded ordinary ply.
 */
static inline Ply make_ordinary_ply(U8 from, U8 to) {
    return make_ply(from, to, ORDINARY_MOVE, (PromotionKind)0);
}

/**
 * @brief Make a pawn promotion ply.
 *
 * @param from Source square index (0–63).
 * @param to Destination square index (0–63).
 * @param promotion Piece to promote to (knight, bishop, rook, or queen).
 *
 * @return 16-bit encoded promotion ply.
 */
static inline Ply make_promotion_ply(
    U8 from,
    U8 to,
    PromotionKind promotion
) {
    return make_ply(from, to, PROMOTION_MOVE, promotion);
}

/**
 * @brief Make an en passant ply.
 *
 * @param from Source square index (0–63).
 * @param to Destination square index (0–63).
 *
 * @return 16-bit encoded en passant ply.
 */
static inline Ply make_en_passant_ply(U8 from, U8 to) {
    return make_ply(from, to, EN_PASSANT_MOVE, (PromotionKind)0);
}

/**
 * @brief Make a castling ply.
 *
 * @param from Source square index of the king (0–63).
 * @param to Destination square index of the king (0–63).
 *
 * @return 16-bit encoded castling ply.
 */
static inline Ply make_castling_ply(U8 from, U8 to) {
    return make_ply(from, to, CASTLING_MOVE, (PromotionKind)0);
}

/**
 * @brief Convert a ply to UCI coordinate notation.
 *
 * @param uci Output buffer to write a move in UCI notation to. The buffer must
 * have a capacity of at least `UCI_PLY_BUFFER_SIZE = 6` bytes.
 * @param ply 16-bit encoded chess ply.
 */
static inline void to_uci_notation(char* uci, const Ply ply) {
    static const char files[] = "abcdefgh";
    static const char promotions[] = "nbrq";
    U16 from = (ply >> 6) & 0x003F;
    U16 to = ply & 0x003F;
    uci[0] = files[from & 7];
    uci[1] = '1' + (from >> 3);
    uci[2] = files[to & 7];
    uci[3] = '1' + (to >> 3);
    uci[4] = ((ply & PLY_KIND_MASK) == PROMOTION_MOVE)
        ? promotions[(ply & PROMOTION_KIND_MASK) >> 12]
        : '\0';
    uci[5] = '\0';
}

#endif
