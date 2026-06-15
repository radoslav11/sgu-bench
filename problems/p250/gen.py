"""Tests for p250 (Constructive Plan): N x M grid (<= 180) of 0/1, find a
maximal C-shaped house (three stacked rectangles, aligned left edges,
middle strictly narrower).

Seed 1 is a 1x1 grid (answer -1), seed 2 an empty 180x180 grid, seed 3 a
random 180x180 grid with scattered trees. Later seeds mix densities and
small sizes, including all-trees grids.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(grid):
    print(len(grid), len(grid[0]))
    for row in grid:
        print(*row)


if seed == 1:
    emit([[0]])
elif seed == 2:
    emit([[0] * 180 for _ in range(180)])
elif seed == 3:
    emit([[1 if rng.random() < 0.08 else 0 for _ in range(180)] for _ in range(180)])
elif seed == 4:
    # Dense trees at full size: tiny or impossible houses.
    emit([[1 if rng.random() < 0.55 else 0 for _ in range(180)] for _ in range(180)])
elif seed == 5:
    emit([[1] * 180 for _ in range(180)])
elif seed == 6:
    # Thin board: 180 x 2, middle block width 1 is forced.
    emit([[1 if rng.random() < 0.1 else 0 for _ in range(2)] for _ in range(180)])
elif seed == 7:
    # Wide board with sparse tree columns.
    emit(
        [
            [1 if (c % 23 == 7 and rng.random() < 0.7) else 0 for c in range(180)]
            for _ in range(7)
        ]
    )
else:
    n = rng.randint(1, 180)
    m = rng.randint(1, 180)
    p = rng.uniform(0.02, 0.4)
    emit([[1 if rng.random() < p else 0 for _ in range(m)] for _ in range(n)])
