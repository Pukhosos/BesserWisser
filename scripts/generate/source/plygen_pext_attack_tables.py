#!.venv/bin/python3.15

"""Generate PEXT-based sliding piece attack tables."""

from argparse import ArgumentParser, Namespace
from collections.abc import Sequence
from sys import stdout
from typing import Final

BOARD_SIZE: Final[int] = 8
SQUARE_COUNT: Final[int] = BOARD_SIZE * BOARD_SIZE

BISHOP_DIRECTIONS: Final[tuple[tuple[int, int], ...]] = (
    (1, 1),
    (1, -1),
    (-1, 1),
    (-1, -1),
)

ROOK_DIRECTIONS: Final[tuple[tuple[int, int], ...]] = (
    (1, 0),
    (-1, 0),
    (0, 1),
    (0, -1),
)


def make_parser() -> ArgumentParser:
    """Make command-line argument parser."""

    parser: ArgumentParser = ArgumentParser(description=__doc__)
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument(
        '--bishop',
        action='store_true',
        help='Generate bishop attack tables',
    )
    group.add_argument(
        '--rook',
        action='store_true',
        help='Generate rook attack tables',
    )
    return parser


def is_on_board(file: int, rank: int) -> bool:
    """Check whether a square is on the board."""

    return 0 <= file < BOARD_SIZE and 0 <= rank < BOARD_SIZE


def get_square_name(square: int) -> str:
    """Convert LERF square index to its enum name."""

    file_index: int = square & 7
    rank_index: int = square >> 3
    file_name: str = chr(ord('A') + file_index)
    return f'{file_name}{rank_index + 1}'


def generate_mask(
    square: int,
    directions: Sequence[tuple[int, int]],
) -> int:
    """Generate relevant occupancy mask for a sliding piece."""

    source_file: int = square & 7
    source_rank: int = square >> 3
    mask: int = 0
    for file_direction, rank_direction in directions:
        file: int = source_file + file_direction
        rank: int = source_rank + rank_direction
        while is_on_board(file, rank):
            next_file: int = file + file_direction
            next_rank: int = rank + rank_direction
            if not is_on_board(next_file, next_rank):
                break
            target_square: int = rank * BOARD_SIZE + file
            mask |= 1 << target_square
            file = next_file
            rank = next_rank
    return mask


def generate_attacks(
    square: int,
    occupancy: int,
    directions: Sequence[tuple[int, int]],
) -> int:
    """Generate sliding piece attacks for a given occupancy."""

    source_file: int = square & 7
    source_rank: int = square >> 3
    attacks: int = 0
    for file_direction, rank_direction in directions:
        file: int = source_file + file_direction
        rank: int = source_rank + rank_direction
        while is_on_board(file, rank):
            target_square: int = rank * BOARD_SIZE + file
            target_bit: int = 1 << target_square
            attacks |= target_bit
            if occupancy & target_bit:
                break
            file += file_direction
            rank += rank_direction
    return attacks


def get_occupancy(subset_index: int, mask: int) -> int:
    """Convert PEXT index into its corresponding occupancy bitboard."""

    occupancy: int = 0
    while mask != 0:
        bit: int = mask & -mask
        if (subset_index & 1) != 0:
            occupancy |= bit
        subset_index >>= 1
        mask ^= bit
    return occupancy


def generate_piece_data(
    directions: Sequence[tuple[int, int]],
) -> tuple[list[int], list[int], list[int]]:
    """Generate masks, offsets, and attacks for a sliding piece."""

    masks: list[int] = [
        generate_mask(square, directions)
        for square in range(SQUARE_COUNT)
    ]  # fmt: skip
    offsets: list[int] = []
    attacks: list[int] = []
    for square, mask in enumerate(masks):
        offsets.append(len(attacks))
        subset_count: int = 1 << mask.bit_count()
        for subset_index in range(subset_count):
            occupancy: int = get_occupancy(subset_index, mask)
            attack: int = generate_attacks(square, occupancy, directions)
            attacks.append(attack)
    return masks, offsets, attacks


def generate_attack_array(
    name: str,
    size: int,
    masks: Sequence[int],
    offsets: Sequence[int],
    attacks: Sequence[int],
) -> list[str]:
    """Generate a flat C attack table."""

    lines: list[str] = [f'const U64 {name}[{size}] = {{']
    for square, mask in enumerate(masks):
        square_name: str = get_square_name(square)
        start: int = offsets[square]
        stop: int = start + (1 << mask.bit_count())
        lines.append(f'    // {square_name}')
        lines.extend(
            f'    0x{attacks[index]:016X}ULL,'
            for index in range(start, stop)
        )  # fmt: skip
    lines.append('};')
    return lines


def generate_piece_source(
    piece: str,
    directions: Sequence[tuple[int, int]],
) -> list[str]:
    """Generate C source for a sliding piece."""

    return generate_attack_array(
        f'{piece}_ATTACK_TABLE',
        {'BISHOP': 5248, 'ROOK': 102400}[piece],
        *generate_piece_data(directions),
    )


def main() -> None:
    """Run the attack table generator."""

    parser: ArgumentParser = make_parser()
    arguments: Namespace = parser.parse_args()
    lines: list[str] = [
        '// This file was generated by a script.',
        '// Do not edit manually.',
        '// See file://./README.md for details.',
        '',
        '#include \"include/plygen/runtimeconst/attack_tables.h\"',
        '',
    ]  # fmt: skip
    if arguments.bishop:
        lines.extend(generate_piece_source('BISHOP', BISHOP_DIRECTIONS))
    else:
        lines.extend(generate_piece_source('ROOK', ROOK_DIRECTIONS))
    lines.append('')
    stdout.write('\n'.join(lines))


if __name__ == '__main__':
    main()
