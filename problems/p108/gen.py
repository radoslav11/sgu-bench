"""Tests for p108 (Self-numbers 2): 1 <= N <= 1e7, 1 <= K <= 5000,
queried indices s_i must satisfy a[s_i] <= N.

The generator sieves self-numbers up to N to learn the exact count and
draw valid indices. Seed 1 is N = 1, K = 1, seeds 2-3 are maximal
(N = 1e7, K = 5000) with uniform vs. clustered/duplicate-heavy query
indices, seed 4 queries every boundary index (1 and the last few), the
rest are random mid sizes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def count_self(n):
    limit = n + 64 + 1
    generated = bytearray(limit + 1)
    small = [0] * 1000
    for i in range(1, 1000):
        small[i] = small[i // 10] + i % 10
    ds_high = 0
    for base in range(0, n + 1, 1000):
        if base:
            ds_high = small[(base // 1000) % 1000] + small[base // 1000000]
        top = min(999, n - base)
        for r in range(0 if base else 1, top + 1):
            generated[base + r + ds_high + small[r]] = 1
    return sum(1 for i in range(1, n + 1) if not generated[i])


def emit(n, queries):
    print(n, len(queries))
    print(" ".join(map(str, queries)))


if seed == 1:
    emit(1, [1])
elif seed == 2:
    total = count_self(10**7)
    emit(10**7, [rng.randint(1, total) for _ in range(5000)])
elif seed == 3:
    total = count_self(10**7)
    qs = [total] * 100 + [1] * 100
    qs += [rng.randint(total - 50, total) for _ in range(2400)]
    qs += [rng.randint(1, 50) for _ in range(2400)]
    rng.shuffle(qs)
    emit(10**7, qs)
elif seed == 4:
    n = 100
    emit(n, [1, 2, 3, 11, 12, 13, 13, 1])
else:
    n = rng.randint(2, 10**6)
    total = count_self(n)
    k = rng.randint(1, min(5000, 50 * total))
    emit(n, [rng.randint(1, total) for _ in range(k)])
