from pathlib import Path
from subprocess import CompletedProcess, run

from pytest import fail, mark

from tests.shared import UNIT_TESTS_PATH, CTestCase, collect_c_tests, get_test_case_id


@mark.parametrize(
    argnames='c_test_case',
    argvalues=[
        c_test_case
        for c_test_file in collect_c_tests(UNIT_TESTS_PATH)
        for c_test_case in c_test_file.tests
    ],
    ids=get_test_case_id,
)
def test_c(native_builds_dir: Path, c_test_case: CTestCase) -> None:
    """Execute a `C` test case and assert that it exits successfully."""

    result: CompletedProcess[str] = run(  # noqa: S603
        c_test_case.get_run_args(native_builds_dir),
        capture_output=True,
        text=True,
        check=False,
    )
    if result.returncode != 0:
        fail(
            reason=f'C test failed:\n{result.stdout}\n{result.stderr}',
            pytrace=False,
        )
