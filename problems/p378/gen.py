"""Tests for p378 (Save the Fisher): N, M <= 500 lake of flow digits 0-4,
then Peter's and the boat's coordinates as "x y" (column, row).

Seed 1 minimal 1x1, seed 2 maximal random flow, seed 3 maximal with the
boat on a huge rectangular current loop (long drift period), later seeds
mix still water, one-way currents that push the boat ashore, narrow
lakes, and a Peter-equals-boat start.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(grid, px, py, bx, by):
    n, m = len(grid), len(grid[0])
    print(n, m)
    for row in grid:
        print("".join(map(str, row)))
    print(px, py)
    print(bx, by)


if seed == 1:
    emit([[0]], 1, 1, 1, 1)
elif seed == 2:
    n = m = 500
    grid = [[rng.randint(0, 4) for _ in range(m)] for _ in range(n)]
    emit(
        grid,
        rng.randint(1, m),
        rng.randint(1, n),
        rng.randint(1, m),
        rng.randint(1, n),
    )
elif seed == 3:
    # clockwise current around the border: 3=right, 2=down, 4=left, 1=up
    n = m = 500
    grid = [[rng.randint(0, 4) for _ in range(m)] for _ in range(n)]
    for c in range(m):
        grid[0][c] = 3
        grid[n - 1][c] = 4
    for r in range(n):
        grid[r][m - 1] = 2
        grid[r][0] = 1
    grid[0][m - 1] = 2
    grid[n - 1][m - 1] = 4
    grid[n - 1][0] = 1
    grid[0][0] = 3
    emit(grid, m // 2, n // 2, 1, 1)
elif seed == 4:
    # still lake, boat never moves, Peter at the opposite corner
    n, m = 500, 500
    grid = [[0] * m for _ in range(n)]
    emit(grid, 1, 1, m, n)
elif seed == 5:
    # everything flows right: the boat exits through the shore
    n, m = rng.randint(2, 300), rng.randint(2, 300)
    grid = [[3] * m for _ in range(n)]
    emit(grid, 1, rng.randint(1, n), m // 2 + 1, rng.randint(1, n))
elif seed == 6:
    # Peter starts exactly on the boat
    n, m = rng.randint(1, 100), rng.randint(1, 100)
    grid = [[rng.randint(0, 4) for _ in range(m)] for _ in range(n)]
    x, y = rng.randint(1, m), rng.randint(1, n)
    emit(grid, x, y, x, y)
elif seed == 7:
    # one-row lake
    n, m = 1, 500
    grid = [[rng.choice([0, 3, 4]) for _ in range(m)]]
    emit(grid, rng.randint(1, m), 1, rng.randint(1, m), 1)
else:
    n, m = rng.randint(2, 200), rng.randint(2, 200)
    grid = [[rng.choice([0, 1, 2, 3, 4]) for _ in range(m)] for _ in range(n)]
    emit(
        grid,
        rng.randint(1, m),
        rng.randint(1, n),
        rng.randint(1, m),
        rng.randint(1, n),
    )
