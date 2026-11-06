#!.venv/bin/python3.15

"""Generate all possible plies in UCI notation."""

from argparse import ArgumentParser, Namespace
from itertools import product
from pathlib import Path
from typing import Final

PROMOTIONS: Final[tuple[str, ...]] = ('q', 'r', 'b', 'n')


def make_parser() -> ArgumentParser:
    """Make command-line argument parser."""

    parser: ArgumentParser = ArgumentParser(description=__doc__)
    parser.add_argument('output_file', type=Path, help='Output file')
    return parser


def get_square_name(file: int, rank: int) -> str:
    """Convert zero-based file and rank coordinates to UCI notation."""

    return f'{chr(ord('a') + file)}{rank + 1}'  # fmt: skip


def generate_moves() -> set[str]:  # noqa: PLR0912
    """Generate all possible UCI moves."""

    moves: set[str] = set()
    for source_file, source_rank in product(range(8), range(8)):
        source_square: str = get_square_name(source_file, source_rank)
        for target_file, target_rank in product(range(8), range(8)):
            file_distance: int = abs(target_file - source_file)
            rank_distance: int = abs(target_rank - source_rank)
            if file_distance != 0 or rank_distance != 0:
                target_square: str = get_square_name(target_file, target_rank)

                # Knight moves
                if (file_distance, rank_distance) in ((1, 2), (2, 1)):
                    moves.add(source_square + target_square)

                # King moves
                if max(file_distance, rank_distance) == 1:
                    moves.add(source_square + target_square)

                # Rook moves
                if file_distance == 0 or rank_distance == 0:
                    moves.add(source_square + target_square)

                # Bishop moves
                if file_distance == rank_distance:
                    moves.add(source_square + target_square)

                # Pawn moves
                for direction, starting_rank, promotion_rank in ((1, 1, 7), (-1, 6, 0)):
                    if target_rank == source_rank + direction:
                        if file_distance == 0:
                            if target_rank == promotion_rank:
                                moves.update(
                                    source_square + target_square + promotion
                                    for promotion in PROMOTIONS
                                )
                            else:
                                moves.add(source_square + target_square)
                        if file_distance == 1:
                            if target_rank == promotion_rank:
                                moves.update(
                                    source_square + target_square + promotion
                                    for promotion in PROMOTIONS
                                )
                            else:
                                moves.add(source_square + target_square)
                    if (
                        source_rank == starting_rank
                        and target_rank == source_rank + 2 * direction
                        and file_distance == 0
                    ):
                        moves.add(source_square + target_square)
    moves.update(('e1g1', 'e1c1', 'e8g8', 'e8c8'))  # Castling
    return moves


def main() -> None:
    """Run the UCI move generator."""

    parser: ArgumentParser = make_parser()
    arguments: Namespace = parser.parse_args()
    output_file: Path = arguments.output_file
    with output_file.open(mode='w', encoding='ascii') as file:
        for move in sorted(generate_moves()):
            file.write(f'{move}\n')


if __name__ == '__main__':
    main()
