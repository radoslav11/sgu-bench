"""Tests for p527 (Explode 'Em All): n,m<=25 grid of '.' and '*'.

Bombs = minimum vertex cover of the bipartite rock graph. Seed 1 is the
minimal 1x1 case. Seeds 2-3 are maximal 25x25: seed 2 is a fully packed
rock grid (answer 25), seed 3 a half-density random grid (large matching).
Later seeds cover the empty grid, a diagonal (forces a big matching), a
block pattern, and random densities.
"""

import random
import sys

MAXN = 25


def emit(grid):
    n = len(grid)
    m = len(grid[0]) if n else 0
    print(n, m)
    sys.stdout.write("\n".join(grid) + "\n")


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit(["*"])
    sys.exit()

if seed == 2:
    # Fully packed 25x25 -> answer 25.
    emit(["*" * MAXN for _ in range(MAXN)])
    sys.exit()

if seed == 3:
    # Maximal 25x25, ~50% density random.
    grid = []
    for _ in range(MAXN):
        grid.append("".join("*" if rng.random() < 0.5 else "." for _ in range(MAXN)))
    emit(grid)
    sys.exit()

if seed == 4:
    # Empty grid -> answer 0.
    n = rng.randint(1, MAXN)
    m = rng.randint(1, MAXN)
    emit(["." * m for _ in range(n)])
    sys.exit()

if seed == 5:
    # Diagonal of rocks -> matching equals min(n,m).
    n = MAXN
    m = MAXN
    grid = []
    for i in range(n):
        grid.append("".join("*" if j == i else "." for j in range(m)))
    emit(grid)
    sys.exit()

if seed == 6:
    # Sparse: a handful of scattered rocks.
    n = rng.randint(1, MAXN)
    m = rng.randint(1, MAXN)
    grid = [["."] * m for _ in range(n)]
    k = rng.randint(1, n * m // 4 + 1)
    for _ in range(k):
        grid[rng.randint(0, n - 1)][rng.randint(0, m - 1)] = "*"
    emit(["".join(r) for r in grid])
    sys.exit()

# Random size and density.
n = rng.randint(1, MAXN)
m = rng.randint(1, MAXN)
p = rng.uniform(0.1, 0.9)
grid = ["".join("*" if rng.random() < p else "." for _ in range(m)) for _ in range(n)]
emit(grid)
