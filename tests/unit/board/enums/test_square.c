#include <assert.h>

#include "include/board/enums/square.h"

static void test_constructors_are_inverses() {
    for (Square original_square = A1; original_square <= H8; original_square++) {
        SquareMask mask = make_square_mask_from_square(original_square);
        Square square_from_mask = make_square_from_square_mask(mask);
        assert(square_from_mask == original_square);
    }
}

#include "tests/headers/dispatch.h"
