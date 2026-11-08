# PEXT Attack Tables

The sliding piece attack tables are generated using
`scripts/generate/source/plygen_pext_attack_tables.py`.

To regenerate these files, run these commands from the project root:

- `BISHOP_ATTACK_TABLE`: PEXT-indexed attack bitboards for all relevant bishop
occupancy subsets.

    ```bash
    scripts/generate/source/plygen_pext_attack_tables.py --bishop \
        > src/plygen/runtimeconst/attack_tables/pext/bishop.c
    ```

- `ROOK_ATTACK_TABLE`: PEXT-indexed attack bitboards for all relevant rook
occupancy subsets.

    ```bash
    scripts/generate/source/plygen_pext_attack_tables.py --rook \
        > src/plygen/runtimeconst/attack_tables/pext/rook.c
    ```

Both tables use Little-Endian Rank-File (LERF) square indexing and occupancy
subsets ordered according to the PEXT bit extraction convention.
