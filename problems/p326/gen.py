"""Tests for p326 (Perspective): 2 <= N <= 20, all values <= 10000.

Input layout: N, win counts w[i], remaining counts r[i], then an NxN
symmetric matrix a[i][j] of remaining intra-division games with zero
diagonal and row sums not exceeding r[i].

Seed 1 is the minimal N=2 all-zero case, seed 2 a maximal dense N=20
test with near-limit game counts, seed 3 a maximal test shaped as a
tight elimination instance. Later seeds cover trivial NO (an opponent
already has too many wins), no remaining games, and random mid cases.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, w, r, a):
    print(n)
    print(*w)
    print(*r)
    for row in a:
        print(*row)


def dense_matrix(n, max_per_pair):
    a = [[0] * n for _ in range(n)]
    for i in range(n):
        for j in range(i + 1, n):
            v = rng.randint(0, max_per_pair)
            a[i][j] = a[j][i] = v
    # row sums with max_per_pair <= 526 stay below 10000 for n = 20
    return a


if seed == 1:
    emit(2, [0, 0], [0, 0], [[0, 0], [0, 0]])
elif seed == 2:
    n = 20
    a = [[0] * n for _ in range(n)]
    for i in range(n):
        for j in range(i + 1, n):
            a[i][j] = a[j][i] = 526
    r = [min(10000, sum(row) + rng.randint(0, 6)) for row in a]
    w = [rng.randint(0, 10000) for _ in range(n)]
    emit(n, w, r, a)
elif seed == 3:
    # Tight instance: every opponent sits exactly at team 1's ceiling, so
    # each remaining opponent-vs-opponent game pushes someone over.
    n = 20
    a = dense_matrix(n, 500)
    for j in range(1, n):
        a[0][j] = a[j][0] = 0
    r = [sum(row) for row in a]
    r[0] = rng.randint(0, 10000)
    cap = 9000 + r[0]
    w = [9000] + [min(10000, cap) for _ in range(n - 1)]
    emit(n, w, r, a)
elif seed == 4:
    # Trivial NO: an opponent already has more wins than team 1 can reach.
    n = rng.randint(2, 20)
    a = dense_matrix(n, 50)
    r = [sum(row) + rng.randint(0, 100) for row in a]
    w = [rng.randint(0, 100) for _ in range(n)]
    w[1] = 10000
    emit(n, w, r, a)
elif seed == 5:
    # No remaining games at all: pure comparison of current win counts.
    n = rng.randint(2, 20)
    a = [[0] * n for _ in range(n)]
    w = [rng.randint(0, 10000) for _ in range(n)]
    w[0] = max(w)
    emit(n, w, [0] * n, a)
else:
    n = rng.randint(2, 20)
    per_pair = rng.choice([1, 3, 25, 200, 500])
    a = dense_matrix(n, per_pair)
    r = [min(10000, sum(row) + rng.randint(0, 30)) for row in a]
    base = rng.randint(0, 9000)
    spread = rng.choice([3, 20, 300])
    w = [
        min(10000, max(0, base + rng.randint(-spread, spread)))
        for _ in range(n)
    ]
    emit(n, w, r, a)
