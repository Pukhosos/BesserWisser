#!.venv/bin/python3.15

"""Generate FEN strings for every position in a compressed PGN archive."""

from __future__ import annotations

from argparse import ArgumentParser, Namespace
from io import TextIOWrapper
from pathlib import Path

from chess import Board
from chess.pgn import Game, read_game
from zstandard import ZstdDecompressor


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
    parser.add_argument(
        '--include-pseudolegal-move-count',
        action='store_true',
        help='Append the number of pseudo-legal moves to each FEN string.',
    )
    return parser


def generate_fens(
    archive_path: Path,
    output_path: Path,
    limit: int,
    include_pseudolegal_move_count: bool = False,
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
            for move in game.mainline_moves():
                board.push(move)
                fen: str = board.fen(en_passant='fen')
                if include_pseudolegal_move_count:
                    fen += f'\t{board.pseudo_legal_moves.count()}'
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
    include_pseudolegal_move_count: bool = arguments.include_pseudolegal_move_count
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
        include_pseudolegal_move_count,
    )


if __name__ == '__main__':
    main()
