"""Tests for p399 (Berodoskar Development): N x M grid (3..15) of '.'
and 'X'. Craters ('X' components) never touch the border, and at least
two craters must be reachable from the ocean through '.' cells.

Seed 1 is the smallest grid that fits two craters, seed 2 a 15x15 grid
packed with many single-cell craters, seed 3 a 15x15 grid with large
ring-shaped craters (nested pockets like the second sample). Later
seeds drop random blobs of varying size and density; every candidate is
validated (and regenerated if needed) against the guarantees.
"""

import random
import sys
from collections import deque

seed = int(sys.argv[1])
rng = random.Random(seed)


def craters_reachable(grid):
    n, m = len(grid), len(grid[0])
    # water over '.' cells from the border
    seen = [[False] * m for _ in range(n)]
    q = deque()
    for i in range(n):
        for j in range(m):
            if grid[i][j] == "." and (i in (0, n - 1) or j in (0, m - 1)):
                seen[i][j] = True
                q.append((i, j))
    wet_adj = set()
    while q:
        r, c = q.popleft()
        for nr, nc in ((r-1, c), (r+1, c), (r, c-1), (r, c+1)):
            if 0 <= nr < n and 0 <= nc < m:
                if grid[nr][nc] == "X":
                    wet_adj.add((nr, nc))
                elif not seen[nr][nc]:
                    seen[nr][nc] = True
                    q.append((nr, nc))
    # count crater components containing a wet-adjacent cell
    label = [[-1] * m for _ in range(n)]
    comps = 0
    reachable = 0
    for i in range(n):
        for j in range(m):
            if grid[i][j] == "X" and label[i][j] == -1:
                cells = [(i, j)]
                label[i][j] = comps
                q2 = deque([(i, j)])
                while q2:
                    r, c = q2.popleft()
                    for nr, nc in ((r-1, c), (r+1, c), (r, c-1), (r, c+1)):
                        if (0 <= nr < n and 0 <= nc < m
                                and grid[nr][nc] == "X"
                                and label[nr][nc] == -1):
                            label[nr][nc] = comps
                            q2.append((nr, nc))
                            cells.append((nr, nc))
                if any(cell in wet_adj for cell in cells):
                    reachable += 1
                comps += 1
    return reachable


def emit(grid):
    print(len(grid), len(grid[0]))
    for row in grid:
        print("".join(row))


def random_grid(n, m, blob_count, blob_size):
    grid = [["."] * m for _ in range(n)]
    for _ in range(blob_count):
        r = rng.randint(1, n - 2)
        c = rng.randint(1, m - 2)
        cells = [(r, c)]
        for _ in range(blob_size - 1):
            r0, c0 = rng.choice(cells)
            nr = r0 + rng.choice((-1, 0, 1))
            nc = c0 + rng.choice((-1, 0, 1))
            if 1 <= nr <= n - 2 and 1 <= nc <= m - 2:
                cells.append((nr, nc))
        for r0, c0 in cells:
            grid[r0][c0] = "X"
    return grid


if seed == 1:
    emit([list(r) for r in [".....", ".X.X.", "....."]])
elif seed == 2:
    # 15x15 sprinkled with isolated single-cell craters
    while True:
        grid = [["."] * 15 for _ in range(15)]
        for i in range(1, 14):
            for j in range(1, 14):
                if i % 2 == 1 and j % 3 == 1 and rng.random() < 0.8:
                    grid[i][j] = "X"
        if craters_reachable(grid) >= 2:
            emit(grid)
            break
elif seed == 3:
    # 15x15 with two big ring craters (interior pockets) plus extras
    grid = [["."] * 15 for _ in range(15)]
    for (r0, c0, h, w) in ((1, 1, 6, 6), (8, 8, 6, 6)):
        for i in range(r0, r0 + h):
            for j in range(c0, c0 + w):
                if i in (r0, r0 + h - 1) or j in (c0, c0 + w - 1):
                    grid[i][j] = "X"
    grid[3][10] = "X"
    grid[11][3] = "X"
    assert craters_reachable(grid) >= 2
    emit(grid)
else:
    while True:
        n = rng.randint(3, 15)
        m = rng.randint(3, 15)
        if min(n, m) < 3 or (n - 2) * (m - 2) < 3:
            continue
        grid = random_grid(
            n, m,
            blob_count=rng.randint(2, 8),
            blob_size=rng.randint(1, 10),
        )
        if craters_reachable(grid) >= 2:
            emit(grid)
            break
