"""Tests for p132 (Another Chocolate Maniac): minimal dominoes on an
M x N cake (M <= 70, N <= 7) so that no two adjacent empty cells remain.

Seed 1 is the 1x1 cake, seed 2 the all-empty 70x7 cake, seed 3 a 70x7
cake with sparse candles, seed 4 the sample. Later seeds vary candle
density (dense, checkered-ish, random) and dimensions.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(grid):
    print(len(grid), len(grid[0]))
    for row in grid:
        print("".join(row))


if seed == 1:
    emit([["."]])
elif seed == 2:
    emit([["." for _ in range(7)] for _ in range(70)])
elif seed == 3:
    emit([["*" if rng.random() < 0.1 else "." for _ in range(7)]
          for _ in range(70)])
elif seed == 4:
    print("5 5")
    print(".*..*")
    print("*....")
    print("..**.")
    print("**.*.")
    print(".**..")
else:
    m = rng.randint(1, 70)
    n = rng.randint(1, 7)
    density = rng.choice([0.0, 0.05, 0.2, 0.4, 0.7])
    grid = [["*" if rng.random() < density else "." for _ in range(n)]
            for _ in range(m)]
    emit(grid)
