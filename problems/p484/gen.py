"""Tests for p484 (Kola): bottle falling through an n x m grid.

Grid cells are '.', '/', '\\' and exactly one 'P'. 1 <= n, m <= 100.
Seed 1 is the 1x1 minimal grid, seeds 2-3 are 100x100 grids with dense
and sparse obstacle layouts, later seeds mix sizes and densities,
including long same-type diagonal slides and walls of obstacles.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, m, grid):
    print(n, m)
    for row in grid:
        print("".join(row))


def random_grid(n, m, p_slash, p_back):
    grid = []
    for _ in range(n):
        row = []
        for _ in range(m):
            r = rng.random()
            if r < p_slash:
                row.append("/")
            elif r < p_slash + p_back:
                row.append("\\")
            else:
                row.append(".")
        grid.append(row)
    pr, pc = rng.randrange(n), rng.randrange(m)
    grid[pr][pc] = "P"
    return grid


if seed == 1:
    emit(1, 1, [["P"]])
elif seed == 2:
    # Maximal grid, dense obstacles of both types.
    n, m = 100, 100
    emit(n, m, random_grid(n, m, 0.35, 0.35))
elif seed == 3:
    # Maximal grid, sparse obstacles so the bottle falls far.
    n, m = 100, 100
    emit(n, m, random_grid(n, m, 0.04, 0.04))
elif seed == 4:
    # Maximal grid made of same-type diagonals: long slides to the right.
    n, m = 100, 100
    grid = [["." for _ in range(m)] for _ in range(n)]
    for i in range(1, n):
        for j in range(m):
            if (i + j) % 3 == 0:
                grid[i][j] = "\\"
    grid[0][0] = "P"
    emit(n, m, grid)
elif seed == 5:
    # Single row: bottle is already in the bottom row.
    m = rng.randint(1, 100)
    row = [rng.choice("./\\") for _ in range(m)]
    row[rng.randrange(m)] = "P"
    emit(1, m, [row])
elif seed == 6:
    # Single column: '/' or '\\' below always hits a wall.
    n = rng.randint(2, 100)
    col = [[rng.choice("./\\")] for _ in range(n)]
    col[rng.randrange(n)] = ["P"]
    emit(n, 1, col)
else:
    n = rng.randint(1, 100)
    m = rng.randint(1, 100)
    p_slash = rng.uniform(0.0, 0.45)
    p_back = rng.uniform(0.0, min(0.45, 0.9 - p_slash))
    emit(n, m, random_grid(n, m, p_slash, p_back))
