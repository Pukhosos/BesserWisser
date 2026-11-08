#include <immintrin.h>

#include "include/aliases/uints.h"
#include "include/board/enums/square.h"
#include "include/plygen/runtimeconst/attack_tables.h"
#include "include/plygen/runtimeconst/relevant_masks.h"

#include "include/plygen/internal/attacks.h"

static const U32 BISHOP_OFFSET[64] = {
    [A1] = 0x00000000ULL,
    [B1] = 0x00000040ULL,
    [C1] = 0x00000060ULL,
    [D1] = 0x00000080ULL,
    [E1] = 0x000000A0ULL,
    [F1] = 0x000000C0ULL,
    [G1] = 0x000000E0ULL,
    [H1] = 0x00000100ULL,
    [A2] = 0x00000140ULL,
    [B2] = 0x00000160ULL,
    [C2] = 0x00000180ULL,
    [D2] = 0x000001A0ULL,
    [E2] = 0x000001C0ULL,
    [F2] = 0x000001E0ULL,
    [G2] = 0x00000200ULL,
    [H2] = 0x00000220ULL,
    [A3] = 0x00000240ULL,
    [B3] = 0x00000260ULL,
    [C3] = 0x00000280ULL,
    [D3] = 0x00000300ULL,
    [E3] = 0x00000380ULL,
    [F3] = 0x00000400ULL,
    [G3] = 0x00000480ULL,
    [H3] = 0x000004A0ULL,
    [A4] = 0x000004C0ULL,
    [B4] = 0x000004E0ULL,
    [C4] = 0x00000500ULL,
    [D4] = 0x00000580ULL,
    [E4] = 0x00000780ULL,
    [F4] = 0x00000980ULL,
    [G4] = 0x00000A00ULL,
    [H4] = 0x00000A20ULL,
    [A5] = 0x00000A40ULL,
    [B5] = 0x00000A60ULL,
    [C5] = 0x00000A80ULL,
    [D5] = 0x00000B00ULL,
    [E5] = 0x00000D00ULL,
    [F5] = 0x00000F00ULL,
    [G5] = 0x00000F80ULL,
    [H5] = 0x00000FA0ULL,
    [A6] = 0x00000FC0ULL,
    [B6] = 0x00000FE0ULL,
    [C6] = 0x00001000ULL,
    [D6] = 0x00001080ULL,
    [E6] = 0x00001100ULL,
    [F6] = 0x00001180ULL,
    [G6] = 0x00001200ULL,
    [H6] = 0x00001220ULL,
    [A7] = 0x00001240ULL,
    [B7] = 0x00001260ULL,
    [C7] = 0x00001280ULL,
    [D7] = 0x000012A0ULL,
    [E7] = 0x000012C0ULL,
    [F7] = 0x000012E0ULL,
    [G7] = 0x00001300ULL,
    [H7] = 0x00001320ULL,
    [A8] = 0x00001340ULL,
    [B8] = 0x00001380ULL,
    [C8] = 0x000013A0ULL,
    [D8] = 0x000013C0ULL,
    [E8] = 0x000013E0ULL,
    [F8] = 0x00001400ULL,
    [G8] = 0x00001420ULL,
    [H8] = 0x00001440ULL,
};

U64 get_bishop_attacks(Square square, U64 occupancy) {
    const U64 index = (
        BISHOP_OFFSET[square]
        + _pext_u64(occupancy, BISHOP_RELEVANT_MASK[square])
    );
    return BISHOP_ATTACK_TABLE[index];
}
