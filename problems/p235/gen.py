"""Tests for p235 (Queen on chessboard): cells reachable in exactly M moves.

N <= 300, M <= 50, board has 'Q', white 'W' (blockers), black 'B'
(capturable, queen stops on them) and '.'. Seed 2 is an empty 300-board
with M = 50 (longest rays), seed 3 a dense 300-board mixing W and B.
Other seeds: fully boxed-in queen (answer 0), serpentine corridors,
M = 0, and random boards.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(m, grid):
    print(len(grid))
    print(m)
    print("\n".join("".join(row) for row in grid))


def random_board(n, pw, pb):
    grid = [
        [
            "W" if rng.random() < pw
            else ("B" if rng.random() < pb else ".")
            for _ in range(n)
        ]
        for _ in range(n)
    ]
    grid[rng.randrange(n)][rng.randrange(n)] = "Q"
    return grid


if seed == 1:
    emit(0, [["Q", "."], [".", "."]])
elif seed == 2:
    grid = [["."] * 300 for _ in range(300)]
    grid[150][150] = "Q"
    emit(50, grid)
elif seed == 3:
    emit(50, random_board(300, 0.1, 0.12))
elif seed == 4:
    # queen boxed in by white pieces -> 0 reachable cells for M > 0
    grid = random_board(300, 0.05, 0.05)
    for i in range(300):
        for j in range(300):
            if grid[i][j] == "Q":
                qi, qj = i, j
    for di in (-1, 0, 1):
        for dj in (-1, 0, 1):
            if (di or dj) and 0 <= qi + di < 300 and 0 <= qj + dj < 300:
                grid[qi + di][qj + dj] = "W"
    emit(7, grid)
elif seed == 5:
    # serpentine corridors of width 1 between full white walls
    n = 300
    grid = [["."] * n for _ in range(n)]
    for i in range(1, n, 2):
        for j in range(n):
            grid[i][j] = "W"
        if (i // 2) % 2 == 0:
            grid[i][n - 1] = "."
        else:
            grid[i][0] = "."
    grid[0][0] = "Q"
    emit(50, grid)
elif seed == 6:
    emit(1, random_board(300, 0.3, 0.3))
elif seed == 7:
    # only black pieces: every move is a capture or a stop
    emit(50, random_board(300, 0.0, 0.5))
else:
    n = rng.randint(2, 300)
    m = rng.randint(0, 50)
    emit(m, random_board(n, rng.uniform(0, 0.25), rng.uniform(0, 0.25)))
