"""Tests for p479 (Funny Feature): n x m grid (<= 200x200), values 1..5.

Solvable grids come from simulating the machine on a permutation of the
cells: the final value of a cell is 1 + number of its neighbours planted
later, which is always in 1..5. Seed 1 is minimal, seed 2 a maximal
random-permutation grid, seed 3 a maximal uniform-random grid (almost
surely unsolvable). Later seeds: snake-order structure, a corrupted
solvable grid, thin strips and random sizes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def grid_from_order(n, m, order):
    rank = [0] * (n * m)
    for pos, idx in enumerate(order):
        rank[idx] = pos
    grid = []
    for i in range(n):
        row = []
        for j in range(m):
            idx = i * m + j
            val = 1
            for di, dj in ((-1, 0), (1, 0), (0, -1), (0, 1)):
                ni, nj = i + di, j + dj
                if 0 <= ni < n and 0 <= nj < m:
                    if rank[ni * m + nj] > rank[idx]:
                        val += 1
            row.append(val)
        grid.append(row)
    return grid


def emit(n, m, grid):
    print(n, m)
    for row in grid:
        print(*row)


if seed == 1:
    emit(1, 1, [[1]])
elif seed == 2:
    n, m = 200, 200
    order = list(range(n * m))
    rng.shuffle(order)
    emit(n, m, grid_from_order(n, m, order))
elif seed == 3:
    n, m = 200, 200
    emit(n, m, [[rng.randint(1, 5) for _ in range(m)] for _ in range(n)])
elif seed == 4:
    # Snake (boustrophedon) planting order: highly structured values.
    n, m = 200, 200
    order = []
    for i in range(n):
        cols = range(m) if i % 2 == 0 else range(m - 1, -1, -1)
        order.extend(i * m + j for j in cols)
    emit(n, m, grid_from_order(n, m, order))
elif seed == 5:
    # Solvable maximal grid with one cell's value changed.
    n, m = 200, 200
    order = list(range(n * m))
    rng.shuffle(order)
    grid = grid_from_order(n, m, order)
    i, j = rng.randrange(n), rng.randrange(m)
    grid[i][j] = rng.choice([v for v in range(1, 6) if v != grid[i][j]])
    emit(n, m, grid)
elif seed == 6:
    n, m = 1, 200
    order = list(range(m))
    rng.shuffle(order)
    emit(n, m, grid_from_order(n, m, order))
elif seed == 7:
    n, m = 200, 1
    order = list(range(n))
    rng.shuffle(order)
    emit(n, m, grid_from_order(n, m, order))
else:
    n = rng.randint(1, 50)
    m = rng.randint(1, 50)
    if rng.random() < 0.6:
        order = list(range(n * m))
        rng.shuffle(order)
        emit(n, m, grid_from_order(n, m, order))
    else:
        emit(n, m, [[rng.randint(1, 5) for _ in range(m)] for _ in range(n)])
