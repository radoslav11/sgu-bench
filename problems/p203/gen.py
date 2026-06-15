"""Tests for p203 (Hyperhuffman): N (2 <= N <= 500000) occurrence counts,
1 <= P_i <= 10^9, given in non-decreasing order.

Seed 1 is minimal (N=2). Seed 2 is maximal with random sorted values.
Seed 3 is maximal with all-equal values (a fully tied, balanced tree).
Seed 4 is maximal with a doubling prefix that forces a deep skewed tree.
Seeds 5+ are mid-size randoms with heavy ties and value mixes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXN = 500000
MAXV = 10**9

if seed == 1:
    n, p = 2, [1, 1]
elif seed == 2:
    n = MAXN
    p = sorted(rng.randint(1, MAXV) for _ in range(n))
elif seed == 3:
    n = MAXN
    p = [MAXV] * n
elif seed == 4:
    # 1, 1, 2, 4, ..., capped at 10^9, then the cap repeated: deep tree
    n = MAXN
    p, v = [1], 1
    while len(p) < n:
        p.append(v)
        v = min(v * 2, MAXV)
elif seed == 5:
    n = MAXN
    p = [1] * n
elif seed == 6:
    # few distinct values: massive ties in the merge queues
    n = rng.randint(100000, MAXN)
    vals = [rng.randint(1, MAXV) for _ in range(5)]
    p = sorted(rng.choice(vals) for _ in range(n))
else:
    n = rng.randint(2, 50000)
    hi = rng.choice([2, 1000, MAXV])
    p = sorted(rng.randint(1, hi) for _ in range(n))

print(n)
print(" ".join(map(str, p)))
