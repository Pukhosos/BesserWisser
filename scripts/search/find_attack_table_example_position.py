#!/usr/bin/env python3

"""Find chess positions demonstrating sliding-piece attack rays."""

from __future__ import annotations

from argparse import ArgumentParser, Namespace
from io import TextIOWrapper
from pathlib import Path
from sys import stderr
from typing import Final

from chess import BISHOP, ROOK, Board, Piece, square_name
from chess.pgn import Game, Headers, read_game
from zstandard import ZstdDecompressor

BOARD_SIZE: Final[int] = 8
SQUARE_COUNT: Final[int] = BOARD_SIZE * BOARD_SIZE

ORTHOGONAL_DIRECTIONS: Final[tuple[tuple[int, int], ...]] = (
    (0, 1),
    (0, -1),
    (1, 0),
    (-1, 0),
)

DIAGONAL_DIRECTIONS: Final[tuple[tuple[int, int], ...]] = (
    (1, 1),
    (1, -1),
    (-1, 1),
    (-1, -1),
)


def make_parser() -> ArgumentParser:
    """Make command-line argument parser."""

    parser: ArgumentParser = ArgumentParser(description=__doc__)
    parser.add_argument(
        'archive',
        type=Path,
        help='Input PGN.ZST archive.',
    )
    return parser


def get_ray(
    board: Board,
    square: int,
    direction: tuple[int, int],
) -> list[tuple[int, Piece]]:
    """Get occupied squares along a geometric ray."""

    file: int = square & 7
    rank: int = square >> 3
    delta_file: int = direction[0]
    delta_rank: int = direction[1]
    ray: list[tuple[int, Piece]] = []
    distance: int = 0
    while True:
        file += delta_file
        rank += delta_rank
        distance += 1
        if not (0 <= file < BOARD_SIZE and 0 <= rank < BOARD_SIZE):
            break
        target_square: int = rank * BOARD_SIZE + file
        piece: Piece | None = board.piece_at(target_square)
        if piece is not None:
            ray.append((distance, piece))
    return ray


def qualifies(
    board: Board,
    square: int,
    piece_type: int,
    opponent_square: int,
) -> bool:
    """Check whether a sliding piece satisfies all six conditions."""

    piece: Piece | None = board.piece_at(square)
    if piece is None or piece.piece_type != piece_type:
        return False
    directions: tuple[tuple[int, int], ...] = (
        ORTHOGONAL_DIRECTIONS if piece_type == ROOK else DIAGONAL_DIRECTIONS
    )
    rays: list[list[tuple[int, Piece]]] = [
        get_ray(board, square, direction)
        for direction in directions
    ]  # fmt: skip

    # 1. At least one ray reaches the board edge without obstruction.
    reaches_edge: bool = any(len(ray) == 0 for ray in rays)

    # 2. At least one ray encounters an opponent's piece first.
    hits_opponent: bool = any(
        len(ray) > 0 and ray[0][1].color != piece.color for ray in rays
    )

    # 3. At least one ray encounters a friendly piece first.
    hits_own: bool = any(
        len(ray) > 0 and ray[0][1].color == piece.color for ray in rays
    )

    # 4. At least one ray contains exactly one occupied square.
    single_piece_ray: bool = any(len(ray) == 1 for ray in rays)

    # 5. At least one ray contains multiple occupied squares.
    multiple_piece_ray: bool = any(len(ray) >= 2 for ray in rays)  # noqa: PLR2004

    # 6. At least one occupied square is not immediately adjacent.
    distant_piece: bool = any(distance > 1 for ray in rays for distance, _ in ray)

    # 7. At least one ray must have an empty square immediately after its first blocker.
    has_empty_square_after_blocker: bool = any(
        len(ray) > 0
        and 0 <= (square & 7) + (ray[0][0] + 1) * direction[0] < BOARD_SIZE
        and 0 <= (square >> 3) + (ray[0][0] + 1) * direction[1] < BOARD_SIZE
        and board.piece_at(
            square + (ray[0][0] + 1) * (direction[1] * BOARD_SIZE + direction[0])
        )
        is None
        for ray, direction in zip(rays, directions, strict=True)
    )

    # 8. Exactly one direction must be blocked by an adjacent piece.
    immediately_blocked_directions: int = sum(
        len(ray) > 0 and ray[0][0] == 1 for ray in rays
    )
    exactly_one_immediate_blocker: bool = immediately_blocked_directions == 1

    # 9. The sliding piece must occupy one of the central 16 squares.
    is_central: bool = (
        2 <= (square & 7) <= 5  # noqa: PLR2004
        and 2 <= (square >> 3) <= 5  # noqa: PLR2004
    )

    # 10. The sliding piece must not attack the opposing sliding piece.
    does_not_attack_opponent: bool = opponent_square not in board.attacks(square)

    return all(
        (
            reaches_edge,
            hits_opponent,
            hits_own,
            single_piece_ray,
            multiple_piece_ray,
            distant_piece,
            has_empty_square_after_blocker,
            exactly_one_immediate_blocker,
            is_central,
            does_not_attack_opponent,
        )
    )


def try_match_position(board: Board) -> tuple[int, int] | None:
    """Find an opposing rook and bishop satisfying all conditions."""

    for rook_color in (True, False):
        bishop_color: bool = not rook_color
        for rook_square in board.pieces(ROOK, rook_color):
            for bishop_square in board.pieces(BISHOP, bishop_color):
                if not qualifies(board, rook_square, ROOK, bishop_square):
                    continue
                if qualifies(board, bishop_square, BISHOP, rook_square):
                    return rook_square, bishop_square
    return None


def print_match(  # noqa: PLR0913, PLR0917
    game: Game,
    game_number: int,
    ply: int,
    board: Board,
    rook_square: int,
    bishop_square: int,
) -> None:
    """Print matching position and source game attribution."""

    headers: Headers = game.headers
    print(f'Archive game: {game_number}')
    print(f'Rook square: {square_name(rook_square).upper()}')
    print(f'Bishop square: {square_name(bishop_square).upper()}')
    print(f'Ply: {ply}')
    print(f'Move: {board.fullmove_number}')
    print(f'FEN: {board.fen()}')
    print(f'White: {headers.get('White', 'Unknown')}')  # fmt: skip
    print(f'Black: {headers.get('Black', 'Unknown')}')  # fmt: skip
    print(f'Result: {headers.get('Result', '*')}')  # fmt: skip
    print(f'Event: {headers.get('Event', 'Unknown')}')  # fmt: skip


def search_archive(archive: Path) -> bool:
    """Search a compressed PGN archive for a qualifying position."""

    game_number: int = 0
    with (
        archive.open(mode='rb') as compressed_file,
        ZstdDecompressor().stream_reader(compressed_file) as stream,
        TextIOWrapper(stream, encoding='utf-8') as pgn_stream,
    ):
        while (game := read_game(pgn_stream)) is not None:
            game_number += 1
            board: Board = game.board()
            for ply, move in enumerate(game.mainline_moves(), start=1):
                board.push(move)
                result: tuple[int, int] | None = try_match_position(board)
                if result is not None:
                    print_match(
                        game,
                        game_number,
                        ply,
                        board,
                        result[0],
                        result[1],
                    )
                    return True
    return False


def main() -> None:
    """Run the position search."""

    parser: ArgumentParser = make_parser()
    arguments: Namespace = parser.parse_args()
    archive: Path = arguments.archive
    if not archive.is_file():
        parser.error(f'Archive does not exist: {archive}')
    found: bool = search_archive(archive)
    if not found:
        print('No qualifying position found.', file=stderr)


if __name__ == '__main__':
    main()
