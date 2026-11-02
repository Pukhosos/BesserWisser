from pathlib import Path
from typing import NamedTuple


class CTestCase(NamedTuple):
    """Identifies a `C` test by its executable name and test index."""

    build_name: str
    test_name: str
    test_index: int
    markers: tuple[str, ...]

    def get_run_args(self, native_builds_dir: Path) -> tuple[str, str]:
        """Return the executable path and test index as subprocess arguments."""

        return (str(native_builds_dir / self.build_name), str(self.test_index))


def get_test_case_id(case: CTestCase) -> str:
    """Return `C` test parametrization id."""

    return f'{case.build_name}::{case.test_name}[{case.test_index}]'
