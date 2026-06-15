"""Tests for p485 (Arrays): partition 3N values into A, B, C maximizing
sum (A_i - B_i) * C_i.

Each test is T then N on the first line, then T lines of 3N values in
[0, 1000]. The allowed (N, T) regimes are 1..10/T<=1000, 11..15/T<=100,
16..20/T<=10, 21..25/T=1. Seed 1 is the smallest case (N=1, T=1), seeds
2-3 are the heaviest single-case regime (N=25, T=1) with different value
distributions, and later seeds sweep the other maximal (N, T) regimes
plus adversarial all-equal and extreme-value inputs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(t, n, rows):
    print(t, n)
    for row in rows:
        print(" ".join(str(x) for x in row))


def rand_rows(t, n, lo=0, hi=1000):
    return [[rng.randint(lo, hi) for _ in range(3 * n)] for _ in range(t)]


if seed == 1:
    emit(1, 1, [[rng.randint(0, 1000) for _ in range(3)]])
elif seed == 2:
    emit(1, 25, rand_rows(1, 25))
elif seed == 3:
    # Second maximal single case: only the extreme values 0 and 1000.
    n = 25
    emit(1, n, [[rng.choice([0, 1000]) for _ in range(3 * n)]])
elif seed == 4:
    # Heaviest multi-case regime in the 16..20 band.
    emit(10, 20, rand_rows(10, 20))
elif seed == 5:
    # Heaviest multi-case regime in the 11..15 band.
    emit(100, 15, rand_rows(100, 15))
elif seed == 6:
    # Heaviest multi-case regime in the 1..10 band.
    emit(1000, 10, rand_rows(1000, 10))
elif seed == 7:
    # All values equal: the answer is forced.
    n = 25
    v = rng.randint(0, 1000)
    emit(1, n, [[v] * (3 * n)])
elif seed == 8:
    # Many small cases with tiny value range to force ties.
    emit(1000, 10, rand_rows(1000, 10, 0, 3))
elif seed == 9:
    # Mid band, values clustered near the maximum.
    emit(100, 13, rand_rows(100, 13, 990, 1000))
else:
    n = rng.choice([1, 5, 8, 12, 18, 22, 25])
    if n <= 10:
        t = rng.randint(1, 1000)
    elif n <= 15:
        t = rng.randint(1, 100)
    elif n <= 20:
        t = rng.randint(1, 10)
    else:
        t = 1

    emit(t, n, rand_rows(t, n))
