#include <immintrin.h>

#include "include/aliases/uints.h"
#include "include/board/enums/square.h"
#include "include/plygen/runtimeconst/attack_tables.h"
#include "include/plygen/runtimeconst/relevant_masks.h"

#include "include/plygen/internal/attacks.h"

#define ROOK_ATTACK_TABLE_SIZE 102400

static const U32 ROOK_OFFSET[64] = {
    [A1] = 0x00000000ULL,
    [B1] = 0x00001000ULL,
    [C1] = 0x00001800ULL,
    [D1] = 0x00002000ULL,
    [E1] = 0x00002800ULL,
    [F1] = 0x00003000ULL,
    [G1] = 0x00003800ULL,
    [H1] = 0x00004000ULL,
    [A2] = 0x00005000ULL,
    [B2] = 0x00005800ULL,
    [C2] = 0x00005C00ULL,
    [D2] = 0x00006000ULL,
    [E2] = 0x00006400ULL,
    [F2] = 0x00006800ULL,
    [G2] = 0x00006C00ULL,
    [H2] = 0x00007000ULL,
    [A3] = 0x00007800ULL,
    [B3] = 0x00008000ULL,
    [C3] = 0x00008400ULL,
    [D3] = 0x00008800ULL,
    [E3] = 0x00008C00ULL,
    [F3] = 0x00009000ULL,
    [G3] = 0x00009400ULL,
    [H3] = 0x00009800ULL,
    [A4] = 0x0000A000ULL,
    [B4] = 0x0000A800ULL,
    [C4] = 0x0000AC00ULL,
    [D4] = 0x0000B000ULL,
    [E4] = 0x0000B400ULL,
    [F4] = 0x0000B800ULL,
    [G4] = 0x0000BC00ULL,
    [H4] = 0x0000C000ULL,
    [A5] = 0x0000C800ULL,
    [B5] = 0x0000D000ULL,
    [C5] = 0x0000D400ULL,
    [D5] = 0x0000D800ULL,
    [E5] = 0x0000DC00ULL,
    [F5] = 0x0000E000ULL,
    [G5] = 0x0000E400ULL,
    [H5] = 0x0000E800ULL,
    [A6] = 0x0000F000ULL,
    [B6] = 0x0000F800ULL,
    [C6] = 0x0000FC00ULL,
    [D6] = 0x00010000ULL,
    [E6] = 0x00010400ULL,
    [F6] = 0x00010800ULL,
    [G6] = 0x00010C00ULL,
    [H6] = 0x00011000ULL,
    [A7] = 0x00011800ULL,
    [B7] = 0x00012000ULL,
    [C7] = 0x00012400ULL,
    [D7] = 0x00012800ULL,
    [E7] = 0x00012C00ULL,
    [F7] = 0x00013000ULL,
    [G7] = 0x00013400ULL,
    [H7] = 0x00013800ULL,
    [A8] = 0x00014000ULL,
    [B8] = 0x00015000ULL,
    [C8] = 0x00015800ULL,
    [D8] = 0x00016000ULL,
    [E8] = 0x00016800ULL,
    [F8] = 0x00017000ULL,
    [G8] = 0x00017800ULL,
    [H8] = 0x00018000ULL,
};

U64 get_rook_attacks(Square square, U64 occupancy) {
    const U64 index =(
        ROOK_OFFSET[square]
        + _pext_u64(occupancy, ROOK_RELEVANT_MASK[square])
    );
    return ROOK_ATTACK_TABLE[index];
}
