"""Tests for p125 (Shtirlits): N <= 3 grid, B[i][j] in 0..4.

Seed 1 is the minimal N=1 case, seeds 2-3 are maximal N=3 grids (one
derived from a real army matrix, one fully random and likely
infeasible). Later seeds alternate between feasible matrices computed
from a random A and arbitrary random B matrices.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def b_from_a(n, a):
    b = [[0] * n for _ in range(n)]
    for i in range(n):
        for j in range(n):
            for di, dj in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                x, y = i + di, j + dj
                if 0 <= x < n and 0 <= y < n and a[x][y] > a[i][j]:
                    b[i][j] += 1
    return b


def emit(n, b):
    print(n)
    for row in b:
        print(" ".join(map(str, row)))


if seed == 1:
    emit(1, [[0]])
elif seed == 2:
    n = 3
    a = [[rng.randint(0, 9) for _ in range(n)] for _ in range(n)]
    emit(n, b_from_a(n, a))
elif seed == 3:
    n = 3
    emit(n, [[rng.randint(0, 4) for _ in range(n)] for _ in range(n)])
else:
    n = rng.randint(1, 3)
    if seed % 2 == 0:
        a = [[rng.randint(0, 9) for _ in range(n)] for _ in range(n)]
        emit(n, b_from_a(n, a))
    else:
        emit(n, [[rng.randint(0, 4) for _ in range(n)] for _ in range(n)])
