"""Tests for p354 (Just Matrix): n <= 600; top[i][j] = larger elements
above in the column, left[i][j] = larger elements before in the row, for
a hidden permutation matrix of 1..n^2; reconstruct any valid A or say 0.

Solvable tests are made by writing down a permutation matrix and encoding
its true top/left matrices. Unsolvable tests use per-cell codes that pass
the local bound checks (top <= i, left <= j) but conflict globally, so
the solver must actually detect the cycle.

Seed 1 is n=1. Seed 2 is n=600 from a uniformly random permutation.
Seed 3 is n=600 with random in-range codes (almost surely inconsistent,
worst case for the cycle check). Seed 4 is the statement sample, seed 5
the fully descending n=600 matrix (all codes at their maxima). Later
seeds are mid/small mixes including an immediate top[i][j] > i reject.
"""

import random
import sys
from bisect import bisect_right, insort

seed = int(sys.argv[1])
rng = random.Random(seed)


def codes_from_matrix(a):
    n = len(a)
    top = [[0] * n for _ in range(n)]
    left = [[0] * n for _ in range(n)]
    for i in range(n):
        seen = []
        for j in range(n):
            left[i][j] = len(seen) - bisect_right(seen, a[i][j])
            insort(seen, a[i][j])
    for j in range(n):
        seen = []
        for i in range(n):
            top[i][j] = len(seen) - bisect_right(seen, a[i][j])
            insort(seen, a[i][j])
    return top, left


def emit(n, top, left):
    print(n)
    for row in top:
        print(" ".join(map(str, row)))
    print()
    for row in left:
        print(" ".join(map(str, row)))


def emit_matrix(a):
    top, left = codes_from_matrix(a)
    emit(len(a), top, left)


def random_perm_matrix(n):
    vals = list(range(1, n * n + 1))
    rng.shuffle(vals)
    return [vals[i * n : (i + 1) * n] for i in range(n)]


if seed == 1:
    emit(1, [[0]], [[0]])
elif seed == 2:
    emit_matrix(random_perm_matrix(600))
elif seed == 3:
    n = 600
    top = [[rng.randint(0, i) for j in range(n)] for i in range(n)]
    left = [[rng.randint(0, j) for j in range(n)] for i in range(n)]
    emit(n, top, left)
elif seed == 4:
    emit(3, [[0, 0, 0], [0, 0, 0], [0, 0, 2]],
         [[0, 0, 0], [0, 1, 0], [0, 1, 2]])  # statement sample
elif seed == 5:
    n = 600
    a = [[n * n - (i * n + j) for j in range(n)] for i in range(n)]
    emit_matrix(a)
elif seed == 6:
    # Immediate local violation: a top entry exceeds its row index.
    n = rng.randint(2, 20)
    a = random_perm_matrix(n)
    top, left = codes_from_matrix(a)
    top[0][rng.randrange(n)] = rng.randint(1, n)
    emit(n, top, left)
elif seed == 7:
    # Row-sorted permutation: left is all zeros, top is arbitrary.
    n = 300
    a = random_perm_matrix(n)
    for row in a:
        row.sort()
    emit_matrix(a)
else:
    n = rng.randint(2, 80)
    if rng.random() < 0.5:
        emit_matrix(random_perm_matrix(n))
    else:
        top = [[rng.randint(0, i) for j in range(n)] for i in range(n)]
        left = [[rng.randint(0, j) for j in range(n)] for i in range(n)]
        emit(n, top, left)
