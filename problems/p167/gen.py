"""Tests for p167 (I-country): N, M <= 15, 0 <= K <= N*M, oil 0..1000.

Seed 1 is the K=0 edge on a 1x1 grid. Seeds 2-3 are maximal 15x15 grids
(K about half the grid is the heaviest DP; K = N*M forces the full
grid). Later seeds add all-zero and all-equal grids (massive ties), a
single row and a single column, and random shapes.
"""

import random
import sys


def emit(n, m, k, grid):
    print(n, m, k)
    for row in grid:
        print(" ".join(map(str, row)))


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit(1, 1, 0, [[rng.randint(0, 1000)]])
elif seed == 2:
    n = m = 15
    emit(n, m, 112, [[rng.randint(0, 1000) for _ in range(m)]
                     for _ in range(n)])
elif seed == 3:
    n = m = 15
    emit(n, m, 225, [[rng.randint(0, 1000) for _ in range(m)]
                     for _ in range(n)])
elif seed == 4:
    n = m = 15
    emit(n, m, 100, [[0] * m for _ in range(n)])
elif seed == 5:
    n = m = 15
    emit(n, m, 113, [[500] * m for _ in range(n)])
elif seed == 6:
    m = 15
    emit(1, m, 7, [[rng.randint(0, 1000) for _ in range(m)]])
elif seed == 7:
    n = 15
    emit(n, 1, 9, [[rng.randint(0, 1000)] for _ in range(n)])
elif seed == 8:
    # high-value ring, low-value center: anti-greedy shape
    n = m = 15
    grid = [[1000 if i in (0, n - 1) or j in (0, m - 1) else rng.randint(0, 5)
             for j in range(m)] for i in range(n)]
    emit(n, m, 60, grid)
else:
    n, m = rng.randint(1, 15), rng.randint(1, 15)
    k = rng.randint(0, n * m)
    emit(n, m, k, [[rng.randint(0, 1000) for _ in range(m)]
                   for _ in range(n)])
