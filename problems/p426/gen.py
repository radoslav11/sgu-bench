"""Tests for p426 (Double cyclic): n, m up to 10^5 (assumed from the
suffix-array reference and 0.75s limit; the statement's bounds were
lost), 1 <= k <= n, 0 <= a_i <= m-1.

Seed 1 is minimal, seed 2 maximal random, seed 3 maximal with a tiny
value alphabet (long tied rotations), then all-equal, binary, periodic
and Thue-Morse patterns that stress lexicographic tie-breaking.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXN = 100000
MAXM = 100000


def emit(n, m, k, a):
    print(n, m, k)
    print(" ".join(map(str, a)))


if seed == 1:
    emit(1, 1, 1, [0])
elif seed == 2:
    n, m = MAXN, MAXM
    emit(n, m, rng.randint(1, n), [rng.randrange(m) for _ in range(n)])
elif seed == 3:
    n, m = MAXN, MAXM
    a = []
    while len(a) < n:
        a.extend([rng.randrange(4)] * rng.randint(1, 30))
    emit(n, m, rng.randint(1, n), a[:n])
elif seed == 4:
    n, m = MAXN, MAXM
    emit(n, m, n, [m - 1] * n)
elif seed == 5:
    n, m = MAXN, 2
    emit(n, m, n, [rng.randrange(2) for _ in range(n)])
elif seed == 6:
    n, m = MAXN, 1000
    period = [rng.randrange(m) for _ in range(5)]
    a = (period * (n // 5 + 1))[:n]
    emit(n, m, rng.randint(1, n), a)
elif seed == 7:
    n, m = MAXN, 7
    a = [bin(i).count("1") % 2 for i in range(n)]
    emit(n, m, 2, a)
elif seed == 8:
    n = rng.randint(1, 12)
    m = rng.randint(1, 8)
    emit(n, m, rng.randint(1, n), [rng.randrange(m) for _ in range(n)])
elif seed == 9:
    n, m = 1, MAXM
    emit(n, m, 1, [rng.randrange(m)])
else:
    n = rng.randint(1000, 50000)
    m = rng.randint(2, MAXM)
    emit(n, m, rng.randint(1, n), [rng.randrange(m) for _ in range(n)])
