"""Tests for p314 (Shortest Paths): k shortest (non-simple) path lengths.

2 <= n <= 10000, 2 <= m <= 50000, 2 <= k <= 10000, directed edges with
lengths 1..1000. Seed 1 is the minimal two-vertex loop, seed 2 the
maximal random graph, seed 3 a maximal chain with one short cycle in
the middle (lengths form arithmetic-like progressions). Seed 4 keeps t
unreachable (all answers NO), seed 5 is a pure parallel-edge bundle,
seed 6 a small DAG with few paths, and the rest are random mid-size.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, edges, k, s, t):
    print(n, len(edges), k)
    print(s, t)
    for a, b, c in edges:
        print(a, b, c)


if seed == 1:
    emit(2, [(1, 2, 5), (2, 1, 7)], 10, 1, 2)
elif seed == 2:
    n, m, k = 10000, 50000, 10000
    edges = []
    for v in range(2, n + 1):
        u = rng.randint(1, v - 1)
        edges.append((u, v, rng.randint(1, 1000)))
    while len(edges) < m:
        a, b = rng.randint(1, n), rng.randint(1, n)
        if a != b:
            edges.append((a, b, rng.randint(1, 1000)))
    emit(n, edges, k, 1, n)
elif seed == 3:
    n, k = 10000, 10000
    edges = [(i, i + 1, 1) for i in range(1, n)]
    mid = n // 2
    edges.append((mid + 1, mid, 1))  # tiny cycle in the middle
    edges.append((mid, mid + 2, 3))
    while len(edges) < 50000:
        a = rng.randint(1, n - 1)
        edges.append((a, a + 1, rng.randint(1, 2)))
    emit(n, edges, k, 1, n)
elif seed == 4:
    n, m, k = 10000, 50000, 10000
    edges = []
    while len(edges) < m:
        a, b = rng.randint(1, n - 1), rng.randint(1, n - 1)
        if a != b:
            edges.append((a, b, rng.randint(1, 1000)))
    emit(n, edges, k, 1, n)  # vertex n has no incoming edges
elif seed == 5:
    n, m, k = 2, 50000, 10000
    edges = [(1, 2, rng.randint(1, 1000)) for _ in range(m)]
    emit(n, edges, k, 1, 2)
elif seed == 6:
    n, k = 6, 10000
    edges = [
        (1, 2, 1), (2, 3, 1), (3, 6, 1), (1, 4, 2), (4, 5, 2), (5, 6, 2),
        (2, 5, 1), (4, 3, 1),
    ]
    emit(n, edges, k, 1, 6)
else:
    n = rng.randint(2, 300)
    m = rng.randint(2, 3000)
    k = rng.randint(2, 10000)
    edges = []
    while len(edges) < m:
        a, b = rng.randint(1, n), rng.randint(1, n)
        if a != b:
            edges.append((a, b, rng.randint(1, 10)))
    s = 1
    t = rng.randint(2, n)
    emit(n, edges, k, s, t)
