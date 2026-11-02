// depends-on: testdata/c_test/example.c

#include <assert.h>
#include <stdlib.h>

#include "testdata/c_test/example.h"

// mark: example
static void test_example() {
    assert(example() == 42);
}

#include "tests/headers/dispatch.h"
