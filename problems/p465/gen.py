"""Tests for p465 (Fire Station Building).

Input: N M R (N <= 100, M <= N(N-1)/2, R <= 10^4), then N fire
probabilities in hundredths of percent summing to 10^4, then M roads
A B L (1 <= A < B <= N, 1 <= L <= 10^4, no duplicate pairs).

Seed 1 is the single-city case, seeds 2-3 complete graphs on 100
cities (R=0 and R>0), seed 4 a disconnected map (-1), seed 6 connected
but every road shorter than 2R (-1), the rest random sparse maps.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def probs(n):
    cuts = sorted(rng.randint(0, 10000) for _ in range(n - 1))
    vals = []
    prev = 0
    for c in cuts:
        vals.append(c - prev)
        prev = c
    vals.append(10000 - prev)
    return vals


def emit(n, m, r, p, edges):
    print(n, m, r)
    print(*p)
    for a, b, l in edges:
        print(a, b, l)


if seed == 1:
    emit(1, 0, 0, [10000], [])
elif seed in (2, 3):
    n = 100
    r = 0 if seed == 2 else 2500
    edges = [
        (a, b, rng.randint(1, 10000))
        for a in range(1, n + 1)
        for b in range(a + 1, n + 1)
    ]
    emit(n, len(edges), r, probs(n), edges)
elif seed == 4:
    n = 100
    edges = []
    for v in range(2, 51):
        edges.append((rng.randint(1, v - 1), v, rng.randint(1, 10000)))
    for v in range(52, n + 1):
        edges.append((rng.randint(51, v - 1), v, rng.randint(1, 10000)))
    edges.sort()
    emit(n, len(edges), rng.choice([0, 100]), probs(n), edges)
elif seed == 5:
    n = 100
    edges = sorted(
        (rng.randint(1, v - 1), v, rng.randint(1, 10000))
        for v in range(2, n + 1)
    )
    emit(n, len(edges), 5000, probs(n), edges)
elif seed == 6:
    n = 100
    edges = sorted(
        (rng.randint(1, v - 1), v, rng.randint(1, 9999))
        for v in range(2, n + 1)
    )
    emit(n, len(edges), 10000, probs(n), edges)
else:
    n = rng.randint(2, 100)
    pairs = [(a, b) for a in range(1, n + 1) for b in range(a + 1, n + 1)]
    m = rng.randint(0, len(pairs))
    chosen = sorted(rng.sample(pairs, m))
    edges = [(a, b, rng.randint(1, 10000)) for a, b in chosen]
    p = probs(n)
    if seed % 2 == 0 and n >= 2:
        p = [0] * n
        p[rng.randrange(n)] = 10000
    emit(n, m, rng.choice([0, 1, 4000, 10000]), p, edges)
