"""Tests for p519 (3D City Model): n, m <= 100 grid of digit heights.

Seed 1 is the minimal 1x1 grid; seeds 2 and 3 are maximal 100x100 grids
with different height distributions (all nines vs. sparse tall spikes);
the rest are random sizes. At least one non-zero digit is guaranteed.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, m, grid):
    # ensure at least one non-zero digit
    if all(ch == "0" for row in grid for ch in row):
        grid[0] = "9" + grid[0][1:]
    print(n, m)
    for row in grid:
        print(row)


if seed == 1:
    emit(1, 1, ["5"])
elif seed == 2:
    n = m = 100
    grid = ["9" * m for _ in range(n)]
    emit(n, m, grid)
elif seed == 3:
    n = m = 100
    grid = []
    for i in range(n):
        row = []
        for j in range(m):
            if (i + j) % 2 == 0:
                row.append(str(rng.randint(0, 9)))
            else:
                row.append("0")
        grid.append("".join(row))
    emit(n, m, grid)
else:
    n = rng.randint(1, 100)
    m = rng.randint(1, 100)
    hi = rng.choice([1, 3, 9])
    grid = ["".join(str(rng.randint(0, hi)) for _ in range(m)) for _ in range(n)]
    emit(n, m, grid)
