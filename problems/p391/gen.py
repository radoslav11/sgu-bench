"""Tests for p391 (Mr. X). The statement's constraint line is truncated;
the editorial is O(k log k), so we assume n, m <= 10^9 and k <= 10^5.

YES instances are built as Cartesian products R x C of 1D-foldable
position sets (isolated marks with odd gaps, optionally border runs).
NO instances perturb such products: remove/add a cell, make a gap even,
insert an odd-length internal run, or break the palindrome condition.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LIM = 10**9
MAXK = 10**5


def foldable_positions(rng, count, lim):
    # Isolated marks separated by random odd gaps >= 3 always pass the
    # 1D folding rules (every in-between pattern is a single gap).
    gaps = [rng.randrange(3, 2 * (lim // (2 * count)) + 1, 2)
            for _ in range(count - 1)]
    start = rng.randint(1, lim - sum(gaps))
    pos = [start]
    for g in gaps:
        pos.append(pos[-1] + g)
    return pos


def emit(n, m, cells):
    out = [f"{n} {m} {len(cells)}"]
    out.extend(f"{x} {y}" for x, y in cells)
    sys.stdout.write("\n".join(out) + "\n")


def product(rows, cols):
    return [(x, y) for x in rows for y in cols]


if seed == 1:
    emit(1, 1, [(1, 1)])
elif seed == 2:
    # Maximal YES: 316 x 316 product of foldable sets, k = 99856.
    rows = foldable_positions(rng, 316, LIM)
    cols = foldable_positions(rng, 316, LIM)
    cells = product(rows, cols)
    rng.shuffle(cells)
    emit(LIM, LIM, cells)
elif seed == 3:
    # Maximal NO: same structure with one product cell replaced by an
    # outsider, so the full-grid check must scan everything.
    rows = foldable_positions(rng, 316, LIM)
    cols = foldable_positions(rng, 316, LIM)
    cells = product(rows, cols)
    cells[rng.randrange(len(cells))] = (rows[0] + 1, cols[0] + 1)
    rng.shuffle(cells)
    emit(LIM, LIM, cells)
elif seed == 4:
    # Maximal random NO: 10^5 distinct random cells.
    cells = set()
    while len(cells) < MAXK:
        cells.add((rng.randint(1, LIM), rng.randint(1, LIM)))
    emit(LIM, LIM, list(cells))
elif seed == 5:
    # Foldable rows/cols but an even gap injected in the rows.
    rows = foldable_positions(rng, 200, LIM)
    rows[100] += 1
    cols = foldable_positions(rng, 200, LIM)
    cells = product(rows, cols)
    rng.shuffle(cells)
    emit(LIM, LIM, cells)
elif seed == 6:
    # Internal runs: even-length runs (YES-compatible) plus border runs.
    rows = [1, 2, 3]                      # run touching the left border
    p = 10
    for _ in range(20):
        rows.extend([p, p + 1])           # internal even runs
        p += rng.randrange(3, 30, 2) + 1  # keep gaps odd
    n = p + rng.randint(0, 50)
    cols = foldable_positions(rng, 25, 10**6)
    cells = product(rows, cols)
    rng.shuffle(cells)
    emit(n, 10**6, cells)
elif seed == 7:
    # Internal odd run of length 3: rows not foldable -> NO.
    rows = [5, 6, 7, 12, 15]
    cols = foldable_positions(rng, 30, 10**6)
    cells = product(rows, cols)
    emit(100, 10**6, cells)
elif seed == 8:
    # Palindrome violation between isolated marks separated by runs:
    # seq = [1, 3, 2, 5, 1] is not mirrored between the two singles.
    rows = [10, 13, 14, 19]
    cols = [4]
    emit(30, 9, product(rows, cols))
else:
    # Small random grids: dense enough that YES and NO both occur.
    n = rng.randint(1, 12)
    m = rng.randint(1, 12)
    all_cells = [(x, y) for x in range(1, n + 1) for y in range(1, m + 1)]
    k = rng.randint(1, min(len(all_cells), 20))
    if rng.random() < 0.5:
        # Random product of small position subsets (often YES).
        rcnt = rng.randint(1, max(1, n // 2))
        ccnt = rng.randint(1, max(1, m // 2))
        rows = sorted(rng.sample(range(1, n + 1), rcnt))
        cols = sorted(rng.sample(range(1, m + 1), ccnt))
        cells = product(rows, cols)
    else:
        cells = rng.sample(all_cells, k)
    emit(n, m, cells)
