"""Tests for p234 (Black-White King Strikes Back): max independent set
on an M x N grid (<= 200 x 200) with removed cells, placement required.

Seed 2 is the full 200 x 200 board, seed 3 a dense random max-size board
(hard for matching), then sparse boards, comb/stripe patterns, an
all-removed board, thin strips, and random mid-size cases.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(grid):
    print(len(grid), len(grid[0]))
    for row in grid:
        print(" ".join(map(str, row)))


if seed == 1:
    emit([[1]])
elif seed == 2:
    emit([[1] * 200 for _ in range(200)])
elif seed == 3:
    emit([
        [1 if rng.random() < 0.9 else 0 for _ in range(200)]
        for _ in range(200)
    ])
elif seed == 4:
    emit([
        [1 if rng.random() < 0.55 else 0 for _ in range(200)]
        for _ in range(200)
    ])
elif seed == 5:
    # comb pattern: full columns joined by one full row
    grid = [[0] * 200 for _ in range(200)]
    for j in range(0, 200, 2):
        for i in range(200):
            grid[i][j] = 1
    grid[0] = [1] * 200
    emit(grid)
elif seed == 6:
    emit([[0]])  # everything removed
elif seed == 7:
    emit([[1 if rng.random() < 0.7 else 0 for _ in range(200)]])  # 1 x 200
else:
    m = rng.randint(1, 200)
    n = rng.randint(1, 200)
    p = rng.uniform(0.2, 1.0)
    emit([
        [1 if rng.random() < p else 0 for _ in range(n)]
        for _ in range(m)
    ])
