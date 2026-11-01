from pathlib import Path
from typing import Final

PROJECT_ROOT: Final[Path] = Path(__file__).parents[2]
"""Path to the root of `BesserWisser` project."""

TESTS_PATH: Final[Path] = PROJECT_ROOT / 'tests'
"""Path to `/tests` folder."""

UNIT_TESTS_PATH: Final[Path] = TESTS_PATH / 'unit'
"""Path to `/tests/unit` folder."""
