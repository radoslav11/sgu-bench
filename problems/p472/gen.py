"""Tests for p472 (Sokoban, one box): maze up to 100 x 100, characters
'#', ' ', '@', '$', '.', each of the last three exactly once, full wall
border keeps the maze closed.

Seed 1 is the minimal one-push maze. Seed 2 is a 100 x 100 serpentine
of 3-wide free bands (guaranteed solvable, thousands of pushes). Seed 3
is a 100 x 100 random maze with sparse walls. Later seeds mix random
densities, an open hall, and unsolvable corner/sealed-room setups.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(grid):
    sys.stdout.write("\n".join("".join(row) for row in grid) + "\n")


def random_maze(rows, cols, wall_p):
    grid = [["#"] * cols for _ in range(rows)]
    for r in range(1, rows - 1):
        for c in range(1, cols - 1):
            if rng.random() >= wall_p:
                grid[r][c] = " "
    free = [
        (r, c)
        for r in range(rows)
        for c in range(cols)
        if grid[r][c] == " "
    ]
    spots = rng.sample(free, 3)
    for (r, c), ch in zip(spots, "@$."):
        grid[r][c] = ch
    return grid


if seed == 1:
    emit([list("#####"), list("#@$.#"), list("#####")])
    sys.exit()

if seed == 2:
    # Serpentine: 3-wide free vertical bands separated by single walls,
    # joined at alternating ends; the box snakes through every band.
    rows, cols = 100, 100
    grid = [["#"] * cols for _ in range(rows)]
    bands = []
    c = 1
    while c + 2 < cols:
        bands.append(c)
        c += 4
    for i, c0 in enumerate(bands):
        for r in range(1, rows - 1):
            for cc in range(c0, c0 + 3):
                grid[r][cc] = " "
    for i in range(len(bands) - 1):
        rows_join = (
            range(rows - 4, rows - 1) if i % 2 == 0 else range(1, 4)
        )
        for r in rows_join:
            for cc in range(bands[i], bands[i + 1] + 3):
                grid[r][cc] = " "
    grid[1][bands[0]] = "@"
    grid[2][bands[0] + 1] = "$"
    last = bands[-1]
    grid[2 if len(bands) % 2 == 0 else rows - 3][last + 1] = "."
    emit(grid)
    sys.exit()

if seed == 3:
    emit(random_maze(100, 100, 0.15))
    sys.exit()

if seed == 4:
    # Open 100 x 100 hall: huge walking BFS areas per push.
    rows, cols = 100, 100
    grid = [["#"] * cols for _ in range(rows)]
    for r in range(1, rows - 1):
        for c in range(1, cols - 1):
            grid[r][c] = " "
    grid[1][1] = "@"
    grid[50][50] = "$"
    grid[rows - 2][cols - 2] = "."
    emit(grid)
    sys.exit()

if seed == 5:
    # Box stuck in a corner: unsolvable.
    grid = random_maze(60, 60, 0.2)
    for r in range(60):
        for c in range(60):
            if grid[r][c] in "@$.":
                grid[r][c] = " "
    grid[1][1] = "$"
    grid[1][2] = " "
    grid[2][1] = " "
    grid[3][3] = "@"
    grid[3][2] = " "
    grid[2][3] = " "
    grid[30][30] = "."
    grid[30][30] = "."
    emit(grid)
    sys.exit()

if seed == 6:
    # Destination sealed inside walls: unsolvable.
    grid = random_maze(80, 80, 0.25)
    for r in range(80):
        for c in range(80):
            if grid[r][c] in "@$.":
                grid[r][c] = " "
    for r in range(39, 44):
        for c in range(39, 44):
            grid[r][c] = "#"
    grid[41][41] = "."
    grid[10][10] = "@"
    grid[10][11] = " "
    grid[10][12] = "$"
    grid[10][13] = " "
    grid[11][11] = " "
    grid[11][12] = " "
    grid[9][11] = " "
    grid[9][12] = " "
    emit(grid)
    sys.exit()

if seed == 7:
    emit(random_maze(100, 100, 0.35))
    sys.exit()

if seed == 8:
    emit(random_maze(50, 50, 0.25))
    sys.exit()

emit(random_maze(rng.randint(5, 20), rng.randint(5, 20), 0.25))
