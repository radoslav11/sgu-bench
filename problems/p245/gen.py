"""Tests for p245 (Black-White Army): N x M board, pieces, walls, one '@'.

N, M <= 300, seven bonus/penalty values in [0, 10000]. Seed 1 is the
minimal 1x1 board, seeds 2-3 are full 300x300 boards with different
piece densities, later seeds mix sizes, wall mazes and bonus patterns.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

PIECES = "PRKBQM"


def emit(n, m, vals, grid):
    print(n, m)
    print(*vals)
    print("\n".join("".join(row) for row in grid))


def random_board(n, m, piece_prob, wall_prob):
    grid = [
        [
            (
                rng.choice(PIECES)
                if rng.random() < piece_prob
                else ("#" if rng.random() < wall_prob else ".")
            )
            for _ in range(m)
        ]
        for _ in range(n)
    ]
    sr, sc = rng.randrange(n), rng.randrange(m)
    grid[sr][sc] = "@"
    return grid


if seed == 1:
    emit(1, 1, [0, 0, 0, 0, 0, 0, 0], [["@"]])
elif seed == 2:
    # Max size, sparse pieces: large safe regions, long closure chains.
    n = m = 300
    vals = [rng.randint(0, 10000) for _ in range(7)]
    emit(n, m, vals, random_board(n, m, 0.02, 0.05))
elif seed == 3:
    # Max size, dense pieces and walls: heavy attack maps.
    n = m = 300
    vals = [rng.randint(0, 10000) for _ in range(7)]
    emit(n, m, vals, random_board(n, m, 0.30, 0.30))
elif seed == 4:
    # Max size, queens only with big penalty: almost everything attacked.
    n = m = 300
    grid = [["." for _ in range(m)] for _ in range(n)]
    for _ in range(400):
        grid[rng.randrange(n)][rng.randrange(m)] = "Q"
    grid[rng.randrange(n)][rng.randrange(m)] = "@"
    emit(n, m, [1, 1, 1, 1, 1, 1, 10000], grid)
elif seed == 5:
    # Empty max board: pure movement, score stays 0.
    n = m = 300
    grid = [["." for _ in range(m)] for _ in range(n)]
    grid[150][150] = "@"
    emit(n, m, [3, 7, 4, 5, 9, 8, 2], grid)
else:
    n = rng.randint(1, 60)
    m = rng.randint(1, 60)
    vals = [rng.randint(0, 10000) for _ in range(7)]
    if seed % 3 == 0:
        # Zero penalty makes every risky capture free.
        vals[6] = 0
    grid = random_board(n, m, rng.uniform(0.05, 0.5), rng.uniform(0.0, 0.4))
    emit(n, m, vals, grid)
