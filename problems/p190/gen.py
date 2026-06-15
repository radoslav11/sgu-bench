"""Tests for p190 (Dominoes): tile board minus removed squares with 2x1.

N <= 40, 0 <= P < N*N, removed squares are distinct. Seed 1 is the
1x1 board (answer No), seed 2 the full empty 40x40 board, seed 3 a
40x40 board with a balanced random set of removed squares (hardest
for the matching). Other seeds: the statement sample, a color-balanced
board with two isolated same-need cells (No despite balanced counts),
an almost fully removed board, sparse/dense random boards, and a comb
pattern that forces long augmenting paths.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, removed):
    removed = list(removed)
    print(n, len(removed))
    for x, y in removed:
        print(x, y)


if seed == 1:
    emit(1, [])
elif seed == 2:
    emit(40, [])
elif seed == 3:
    # balanced random removal: same number of each color, ~25% density
    n = 40
    cells = [(x, y) for x in range(1, n + 1) for y in range(1, n + 1)]
    black = [c for c in cells if (c[0] + c[1]) % 2 == 0]
    white = [c for c in cells if (c[0] + c[1]) % 2 == 1]
    k = 200
    removed = rng.sample(black, k) + rng.sample(white, k)
    rng.shuffle(removed)
    emit(n, removed)
elif seed == 4:
    emit(4, [(1, 3), (1, 2), (1, 1), (2, 1), (3, 1), (4, 1),
             (3, 2), (4, 2), (3, 3), (4, 3)])
elif seed == 5:
    # color counts balanced, but two opposite-color cells are isolated
    # in different corners -> No
    n = 40
    removed = [(1, 2), (2, 1), (1, 39), (2, 40)]
    emit(n, removed)
elif seed == 6:
    # all but two adjacent squares removed -> Yes with a single domino
    n = 40
    keep = {(17, 23), (18, 23)}
    removed = [(x, y) for x in range(1, n + 1) for y in range(1, n + 1)
               if (x, y) not in keep]
    rng.shuffle(removed)
    emit(n, removed)
elif seed == 7:
    # comb: vertical walls with gaps, long corridors for augmenting paths
    n = 40
    removed = []
    for x in range(2, n, 2):
        gap = rng.randint(1, n)
        removed += [(x, y) for y in range(1, n + 1) if y != gap]
    emit(n, removed)
elif seed == 8:
    # imbalanced colors: remove only one color -> No, full matching run
    n = 40
    cells = [(x, y) for x in range(1, n + 1) for y in range(1, n + 1)
             if (x + y) % 2 == 0]
    removed = rng.sample(cells, 30)
    emit(n, removed)
else:
    n = rng.randint(2, 40)
    cells = [(x, y) for x in range(1, n + 1) for y in range(1, n + 1)]
    p = rng.randint(0, min(len(cells) - 1, int(len(cells) * rng.uniform(0.05, 0.6))))
    removed = rng.sample(cells, p)
    emit(n, removed)
