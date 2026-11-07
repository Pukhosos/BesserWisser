# Generated Testdata

## How To Reproduce

- `lichess-games-pseudo-legal-moves.fen`:

    Download `lichess_db_standard_rated_2014-09.pgn.zst` into `testdata/chess`
    and generate `1_000_000` unique FENs with the number of pseudo legal moves:

    ```sh
    scripts/generate/testdata/fen_from_lichess_archive.py \
        testdata/chess/lichess_db_standard_rated_2014-09.pgn.zst \
        -o testdata/generated/lichess-games-pseudo-legal-moves-unsorted.fen \
        -n 1_105_728 \
        --include-pseudo-legal-move-count \
    && LC_ALL=C sort -u -S 1G \
        testdata/generated/lichess-games-pseudo-legal-moves-unsorted.fen \
        > testdata/generated/lichess-games-pseudo-legal-moves.fen \
    && rm testdata/generated/lichess-games-pseudo-legal-moves-unsorted.fen
    ```

- `lichess-games.fen`

    Download `lichess_db_standard_rated_2014-09.pgn.zst` into `testdata/chess`
    and generate `1_000_000` unique FENs:

    ```sh
    scripts/generate/testdata/fen_from_lichess_archive.py \
        testdata/chess/lichess_db_standard_rated_2014-09.pgn.zst \
        -o testdata/generated/lichess-games-unsorted.fen \
        -n 1_105_728 \
    && LC_ALL=C sort -u -S 1G \
        testdata/generated/lichess-games-unsorted.fen \
        > testdata/generated/lichess-games.fen \
    && rm testdata/generated/lichess-games-unsorted.fen
    ```

- `uci-plies.txt`

    Generate all possible plies in UCI notation with:

    ```sh
    scripts/generate/testdata/all_uci_plies.py testdata/generated/uci-plies.txt
    ```
