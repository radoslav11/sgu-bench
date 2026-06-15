"""Tests for p406 (Goggle): n, m, k, l <= 10, values 1..100.

A query is l numbers b with distinct absolute values, 1 <= |b| <= 100,
b != 0. Seed 1 is minimal. Seeds 2-3 are maximal (n=m=10, k=l=10);
seed 2 draws values from 1..100, seed 3 from a tiny pool {1..5} so
sequences share elements and queries match often. Seed 6 makes all
query terms negative, seed 7 plants a required value absent from every
sequence so queries match nothing; other seeds are random sizes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1)
    print(1, 1)
    print(1, 1)
    sys.exit()

if seed == 2:
    n, m, pool, neg = 10, 10, list(range(1, 101)), 0.3
elif seed == 3:
    n, m, pool, neg = 10, 10, list(range(1, 6)), 0.3
elif seed == 7:
    n, m, pool, neg = 10, 10, list(range(1, 8)), 0.3
else:
    n, m = rng.randint(1, 10), rng.randint(1, 10)
    pool = list(range(1, rng.choice([8, 20, 101])))
    neg = 1.0 if seed == 6 else 0.4

print(n, m)
for _ in range(n):
    k = rng.randint(1, 10)
    print(k, *[rng.choice(pool) for _ in range(k)])

for _ in range(m):
    l = rng.randint(1, min(10, len(pool)))
    absvals = rng.sample(pool, l)
    q = [v if rng.random() >= neg else -v for v in absvals]
    if seed == 7:
        q[0] = 100  # required value never present in any sequence
    print(len(q), *q)
