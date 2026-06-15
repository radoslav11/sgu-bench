"""Tests for p544 (Chess Championship).

Input: line 1 "n k" (1<=n<=500, 1<=k<=n); line 2 n distinct levels for
Berland; line 3 n distinct levels for Byteland. All 2n levels are distinct
integers from 0 to 1e9.

Seed 1 is the minimal n=1 case. Seeds 2-3 are maximal n=500 with k=2
(the gap parity that maximizes the win_a * win_b DP table) under two
different value distributions. Later seeds vary n, k and value spreads,
including cases where (n-k) is odd (answer 0).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXV = 10**9


def distinct_levels(count, rng):
    return rng.sample(range(0, MAXV + 1), 2 * count)


if seed == 1:
    print(1, 1)
    print(0)
    print(1)
    sys.exit()

if seed in (2, 3):
    n = 500
    k = 2
    levels = distinct_levels(n, rng)
    if seed == 3:
        # cluster all of one team below the other team's range so the
        # combined sort interleaves differently
        levels = list(range(0, 2 * n))
        rng.shuffle(levels)
    a = levels[:n]
    b = levels[n:]
    print(n, k)
    print(*a)
    print(*b)
    sys.exit()

n = rng.randint(1, 500)
k = rng.randint(1, n)
levels = distinct_levels(n, rng)
a = levels[:n]
b = levels[n:]
print(n, k)
print(*a)
print(*b)
