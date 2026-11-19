#!.venv/bin/python3.15

"""Generate FEN strings for every position in a compressed PGN archive."""

from __future__ import annotations

from argparse import ArgumentParser, Namespace
from io import TextIOWrapper
from itertools import chain, pairwise
from pathlib import Path
from typing import Final

from chess import Board, Move
from chess.pgn import Game, read_game
from zstandard import ZstdDecompressor

NULL_MOVE: Final[Move] = Move.null()


def make_parser() -> ArgumentParser:
    """Make command-line argument parser."""

    parser: ArgumentParser = ArgumentParser(
        description='Generate FEN strings from a PGN archive.',
    )
    parser.add_argument('archive', type=Path, help='Input PGN.ZST archive.')
    parser.add_argument(
        '-o',
        '--output',
        type=Path,
        required=True,
        help='Output file for FEN strings.',
    )
    parser.add_argument(
        '-n',
        '--number',
        type=int,
        required=True,
        help='Number of FEN strings to generate.',
    )
    group = parser.add_mutually_exclusive_group()
    group.add_argument(
        '--include-legal-move-count',
        action='store_true',
        help='Append the number of legal moves to each FEN string.',
    )
    group.add_argument(
        '--include-pseudolegal-move-count',
        action='store_true',
        help='Append the number of pseudo-legal moves to each FEN string.',
    )
    group.add_argument(
        '--include-transitions',
        action='store_true',
        help='Append a Ply encoded as an integer followed by the resulting FEN.',
    )
    return parser


def move_to_ply(board: Board, move: Move) -> int:
    """Convert a python-chess move to a 16-bit encoded ply."""

    from_square: int = move.from_square
    to_square: int = move.to_square
    promotion: int = (
        {
            2: 0x0000,  # KNIGHT_PROMOTION
            3: 0x1000,  # BISHOP_PROMOTION
            4: 0x2000,  # ROOK_PROMOTION
            5: 0x3000,  # QUEEN_PROMOTION
        }[move.promotion]
        if move.promotion is not None
        else 0
    )
    ply_kind: int = (
        0x4000
        if move.promotion is not None
        else 0x8000
        if board.is_en_passant(move)
        else 0xC000
        if board.is_castling(move)
        else 0x0000
    )
    return ply_kind | promotion | (from_square << 6) | to_square


def generate_fens(  # noqa: PLR0913, PLR0917
    archive_path: Path,
    output_path: Path,
    limit: int,
    include_legal_move_count: bool = False,
    include_pseudolegal_move_count: bool = False,
    include_transition: bool = False,
) -> None:
    """Generate FEN strings for all mainline positions in a PGN archive."""

    count: int = 0
    with (
        archive_path.open(mode='rb') as archive_file,
        ZstdDecompressor().stream_reader(archive_file) as decompressed_stream,
        TextIOWrapper(decompressed_stream, encoding='utf-8') as pgn_stream,
        output_path.open(mode='w', encoding='utf-8') as output_file,
    ):
        game: Game | None = read_game(pgn_stream)
        while game is not None:
            board: Board = game.board()
            for move, next_move in pairwise(chain(game.mainline_moves(), [NULL_MOVE])):
                board.push(move)
                fen: str = board.fen(en_passant='fen')
                if include_legal_move_count:
                    fen += f'\t{board.legal_moves.count()}'
                if include_pseudolegal_move_count:
                    fen += f'\t{board.pseudo_legal_moves.count()}'
                if include_transition:
                    if next_move == NULL_MOVE:
                        break
                    fen += f'\t{move_to_ply(board, next_move)}'
                    board.push(next_move)
                    fen += f'\t{board.fen(en_passant='fen')}'  # fmt: skip
                    board.pop()
                output_file.write(fen + '\n')
                count += 1
                if count == limit:
                    return None
            game = read_game(pgn_stream)


def main() -> None:
    """Run the FEN generator."""

    parser: ArgumentParser = make_parser()
    arguments: Namespace = parser.parse_args()
    archive_path: Path = arguments.archive
    output_path: Path = arguments.output
    game_count: int = arguments.number
    include_legal_move_count: bool = arguments.include_legal_move_count
    include_pseudolegal_move_count: bool = arguments.include_pseudolegal_move_count
    include_transitions: bool = arguments.include_transitions
    if not archive_path.is_file():
        parser.error(f'Archive does not exist: {archive_path}')
    if not output_path.parent.is_dir():
        parser.error(f'Output directory does not exist: {output_path.parent}')
    if archive_path.resolve() == output_path.resolve():
        parser.error('Input and output paths must be different.')
    generate_fens(
        archive_path,
        output_path,
        game_count,
        include_legal_move_count,
        include_pseudolegal_move_count,
        include_transitions,
    )


if __name__ == '__main__':
    main()
