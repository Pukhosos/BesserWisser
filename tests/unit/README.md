# Native Unit Tests

Native C tests are run with pytest rather than a separate C testing framework.
This keeps test execution, reporting, and failure handling consistent across
Python and C tests without introducing another build or test orchestration
system.

## Writing Tests

Create a `test_*.c` file in this directory or one of its subdirectories. Define
each test as a `static void` function with a name starting with `test_`, and
make sure `tests/headers/dispatch.h` is included at the end of the file:

```c
#include <assert.h>

static void test_addition() {
    assert(1 + 2 == 3);
}

#include "tests/headers/dispatch.h"
```

The `tests/headers/dispatch.h` macro is required for the test framework to generate and
inject the `main()` function responsible for dispatching individual tests. It is
supplied automatically at the beginning of the testsuite run.

Each function is discovered automatically and executed as a separate pytest test
case. A nonzero exit code, including one caused by a failed assertion,
compilation error, etc. marks the test as failed.

### Declaring Dependencies

If a test requires additional `.c` source files, declare them at the very
beginning of the test file:

```c
// depends-on: src/example.c

#include <assert.h>

#include "include/example.h"

static void test_example() {
    assert(example() == 42);
}

#include "tests/headers/dispatch.h"
```

Include pnaths should be relative to the repository root. All dependency
declarations must be consecutive and appear before any other lines.

### Adding Pytest Markers

Individual C tests can be assigned pytest markers using `// mark:` comments
placed immediately above their declarations:

```c
#include <assert.h>

#include "include/example.h"

// mark: example
static void test_example() {
    assert(example() == 42);
}

#include "tests/headers/dispatch.h"
```

Multiple markers can be assigned to the same test by placing consecutive
marker comments above its declaration.

## Running Tests

Run unit `C` tests using pytest:

```sh
pytest tests/unit/ -n 8
```

**note:** Test executables are compiled automatically once per pytest session
with `AddressSanitizer` and `UndefinedBehaviorSanitizer` enabled. No manual
compilation or cleanup is required.
