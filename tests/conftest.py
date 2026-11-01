from pathlib import Path

from pytest import fixture

from tests.shared import PROJECT_ROOT, TESTS_PATH, UNIT_TESTS_PATH


@fixture(scope='session')
def project_root() -> Path:
    """Path to the root of `GrushaChess`."""

    return PROJECT_ROOT


@fixture(scope='session')
def tests_path() -> Path:
    """Path to `/tests` folder."""

    return TESTS_PATH


@fixture(scope='session')
def unit_tests_path() -> Path:
    """Path to `/tests/unit` folder."""

    return UNIT_TESTS_PATH
