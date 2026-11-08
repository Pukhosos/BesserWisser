
#ifndef PLYGEN_RUNTIMECONST_ATTACK_TABLES_H
#define PLYGEN_RUNTIMECONST_ATTACK_TABLES_H

#include "include/aliases/uints.h"

/**
 * @brief Precomputed bishop attack bitboards.
 *
 * For example, consider this position:
 *
 * ```
 *   +------------------------+
 * 8 | r  .  .  q  r  .  k  . |
 * 7 | .  p  p  .  .  p  p  p |
 * 6 | .  .  .  p  .  .  .  . |
 * 5 | .  .  n  N  b  .  .  . |
 * 4 | p  .  R  .  P  .  .  . |
 * 3 | P  .  .  .  B  P  .  P |
 * 2 | .  P  .  .  Q  .  P  . |
 * 1 | .  .  .  .  .  R  K  . |
 *   +------------------------+
 *     a  b  c  d  e  f  g  h
 * ```
 *
 * Source:
 *
 *   - MisterBiggStuff (White) vs. nuwonga (Black), 0-1;
 *
 *   - Lichess Rated Bullet game, September 2014 archive, game 478, ply 39;
 *
 *   - FEN: `r2qr1k1/1pp2ppp/3p4/2nNb3/p1R1P3/P3BP1P/1P2Q1P1/5RK1 b - - 2 20`.
 *
 * The position was found with
 *
 * ```
 * scripts/search/find_attack_table_example_position.py \
 *     testdata/chess/lichess_db_standard_rated_2014-09.pgn.zst
 * ```
 *
 * The black bishop on E5 attacks nine squares:
 *
 * ```
 *   +------------------------+
 * 8 | .  .  .  .  .  .  .  . |
 * 7 | .  .  .  .  .  .  *  . |
 * 6 | .  .  .  *  .  *  .  . |
 * 5 | .  .  .  .  b  .  .  . |
 * 4 | .  .  .  *  .  *  .  . |
 * 3 | .  .  *  .  .  .  *  . |
 * 2 | .  *  .  .  .  .  .  * |
 * 1 | .  .  .  .  .  .  .  . |
 *   +------------------------+
 *     a  b  c  d  e  f  g  h
 * ```
 *
 * The resulting attack bitboard is `0x0040280028448200`.
 *
 * Here, `b` marks the bishop and `*` marks an attacked square.
 *
 * The northwest ray stops immediately at the friendly pawn on D6. The northeast
 * ray passes through F6 and stops at the friendly pawn on G7. The southwest ray
 * passes through D4 and C3 before reaching the opposing pawn on B2. The
 * southeast ray reaches the board edge through F4, G3, and H2 without
 * obstruction.
 */
extern const U64 BISHOP_ATTACK_TABLE[];

/**
 * @brief Precomputed rook attack bitboards.
 *
 * For example, consider this position:
 *
 * ```
 *   +------------------------+
 * 8 | r  .  .  q  r  .  k  . |
 * 7 | .  p  p  .  .  p  p  p |
 * 6 | .  .  .  p  .  .  .  . |
 * 5 | .  .  n  N  b  .  .  . |
 * 4 | p  .  R  .  P  .  .  . |
 * 3 | P  .  .  .  B  P  .  P |
 * 2 | .  P  .  .  Q  .  P  . |
 * 1 | .  .  .  .  .  R  K  . |
 *   +------------------------+
 *     a  b  c  d  e  f  g  h
 * ```
 *
 * Source:
 *
 *   - MisterBiggStuff (White) vs. nuwonga (Black), 0-1;
 *
 *   - Lichess Rated Bullet game, September 2014 archive, game 478, ply 39;
 *
 *   - FEN: `r2qr1k1/1pp2ppp/3p4/2nNb3/p1R1P3/P3BP1P/1P2Q1P1/5RK1 b - - 2 20`.
 *
 * The position was found with
 *
 * ```
 * scripts/search/find_attack_table_example_position.py \
 *     testdata/chess/lichess_db_standard_rated_2014-09.pgn.zst
 * ```
 *
 * The white rook on C4 attacks eight squares:
 *
 * ```
 *   +------------------------+
 * 8 | .  .  .  .  .  .  .  . |
 * 7 | .  .  .  .  .  .  .  . |
 * 6 | .  .  .  .  .  .  .  . |
 * 5 | .  .  *  .  .  .  .  . |
 * 4 | *  *  R  *  *  .  .  . |
 * 3 | .  .  *  .  .  .  .  . |
 * 2 | .  .  *  .  .  .  .  . |
 * 1 | .  .  *  .  .  .  .  . |
 *   +------------------------+
 *     a  b  c  d  e  f  g  h
 * ```
 *
 * The resulting attack bitboard is `0x000000041B040404`.
 *
 * Here, `R` marks the rook and `*` marks an attacked square.
 *
 * The north ray stops immediately at the opposing knight on C5. The east ray
 * passes through D4 and stops at the friendly pawn on E4. The west ray passes
 * through B4 and stops at the opposing pawn on A4. The south ray reaches the
 * board edge through C3, C2, and C1 without obstruction.
 */
extern const U64 ROOK_ATTACK_TABLE[];

#endif
