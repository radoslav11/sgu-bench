"""Tests for p421 (k-th Product): n m k then n values a_i.

1 <= n, k <= 10000, 1 <= m <= 13, k <= C(n, m), -1e6 <= a_i <= 1e6.
Seed 1 is the minimal case, seeds 2-3 maximal n/m/k with different value
mixes; later seeds stress zeros, ones, signs, duplicates and small n.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXV = 10 ** 6


def cap_k(n, m, want):
    total = math.comb(n, m)
    return max(1, min(want, min(total, 10000)))


def emit(n, m, k, vals):
    print(n, m, k)
    print(" ".join(map(str, vals)))


if seed == 1:
    emit(1, 1, 1, [rng.choice([-MAXV, 0, MAXV])])
elif seed == 2:
    # maximal, mixed random values including extremes
    n, m = 10000, 13
    vals = [rng.randint(-MAXV, MAXV) for _ in range(n)]
    vals[0], vals[1] = MAXV, -MAXV
    emit(n, m, cap_k(n, m, 10000), vals)
elif seed == 3:
    # maximal with heavy duplicates and many zeros
    n, m = 10000, 13
    pool = [-MAXV, -2, -1, 0, 0, 1, 2, MAXV]
    vals = [rng.choice(pool) for _ in range(n)]
    emit(n, m, cap_k(n, m, rng.randint(1, 10000)), vals)
elif seed == 4:
    # all negative, odd m -> negative products
    n, m = 10000, 13
    vals = [rng.randint(-MAXV, -1) for _ in range(n)]
    emit(n, m, cap_k(n, m, 10000), vals)
elif seed == 5:
    # all values equal (massive ties)
    n, m = 10000, rng.randint(1, 13)
    v = rng.choice([-3, -1, 0, 1, 7])
    emit(n, m, cap_k(n, m, rng.randint(1, 10000)), [v] * n)
elif seed == 6:
    # m == n (single tuple)
    n = rng.randint(1, 13)
    vals = [rng.randint(-MAXV, MAXV) for _ in range(n)]
    emit(n, n, 1, vals)
elif seed == 7:
    # only -1, 0, 1
    n, m = 10000, rng.randint(2, 13)
    vals = [rng.choice([-1, 0, 1]) for _ in range(n)]
    emit(n, m, cap_k(n, m, 10000), vals)
elif seed == 8:
    # k at the very end of the list (k = min(C(n,m), 10000))
    n = rng.randint(14, 30)
    m = rng.randint(2, 13)
    vals = [rng.randint(-9, 9) for _ in range(n)]
    emit(n, m, cap_k(n, m, 10 ** 9), vals)
else:
    n = rng.randint(13, 10000)
    m = rng.randint(1, 13)
    vals = [rng.randint(-MAXV, MAXV) for _ in range(n)]
    emit(n, m, cap_k(n, m, rng.randint(1, 10000)), vals)
