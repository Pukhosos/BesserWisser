#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "include/aliases/uints.h"
#include "include/plystruct/constants.h"
#include "include/plystruct/ply.h"

#ifndef UCI_TESTDATA
#define UCI_TESTDATA "testdata/generated/uci-plies.txt"
#endif

static Ply make_ply_from_uci_notation(const char* uci) {
    size_t length = strlen(uci);
    U8 from = (U8)((uci[0] - 'a') + (uci[1] - '1') * 8);
    U8 to   = (U8)((uci[2] - 'a') + (uci[3] - '1') * 8);
    if (length == 5) {
        PromotionKind promotion;
        switch (uci[4]) {
            case 'n': promotion = KNIGHT_PROMOTION; break;
            case 'b': promotion = BISHOP_PROMOTION; break;
            case 'r': promotion = ROOK_PROMOTION;   break;
            case 'q': promotion = QUEEN_PROMOTION;  break;
        }
        return make_promotion_ply(from, to, promotion);
    }
    return make_ordinary_ply(from, to);
}

static void test_to_uci_notation() {
    FILE *file = fopen(UCI_TESTDATA, "r");
    assert(file != NULL);
    char original_uci[UCI_PLY_BUFFER_SIZE + 1];
    char serialized_uci[UCI_PLY_BUFFER_SIZE];
    while (fgets(original_uci, sizeof(original_uci), file) != NULL) {
        original_uci[strcspn(original_uci, "\r\n")] = '\0';
        if (original_uci[0] == '\0') break;
        Ply ply = make_ply_from_uci_notation(original_uci);
        to_uci_notation(serialized_uci, ply);
        assert(strcmp(original_uci, serialized_uci) == 0);
    }
    assert(fclose(file) == 0);
}

#include "tests/headers/dispatch.h"
