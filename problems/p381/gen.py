"""Tests for p381 (Bidirected Graph). The statement's printed constraints
are garbled, so we use a moderate n <= 100000, m <= 200000 (memory limit
262144 KB and a linear-time algorithm suggest a large graph).

Seed 1 minimal, seed 2 maximal solvable (signs derived from a hidden
flip assignment, so the answer is YES), seed 3 maximal random signs
(odd-product cycles make it NO), later seeds mix solvable/unsolvable
mid-size graphs, multi-edges, isolated vertices and a single big tree.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, edges):
    print(n, len(edges))
    for a, b, d1, d2 in edges:
        print(a, b, d1, d2)


def solvable_edges(n, m, rng):
    # hidden assignment: vertex set S to flip; build edges that become
    # directed after flipping S
    hidden = [rng.randint(0, 1) for _ in range(n + 1)]
    edges = []
    for _ in range(m):
        a = rng.randint(1, n)
        b = rng.randint(1, n)
        while b == a:
            b = rng.randint(1, n)
        d1 = rng.choice([-1, 1])
        d2 = -d1
        if hidden[a]:
            d1 = -d1
        if hidden[b]:
            d2 = -d2
        edges.append((a, b, d1, d2))
    return edges


if seed == 1:
    emit(2, [(1, 2, 1, 1)])
elif seed == 2:
    n, m = 100000, 200000
    emit(n, solvable_edges(n, m, rng))
elif seed == 3:
    n, m = 100000, 200000
    edges = []
    for _ in range(m):
        a = rng.randint(1, n)
        b = rng.randint(1, n)
        while b == a:
            b = rng.randint(1, n)
        edges.append((a, b, rng.choice([-1, 1]), rng.choice([-1, 1])))
    emit(n, edges)
elif seed == 4:
    # big random tree with random signs: always YES (no cycles)
    n = 100000
    edges = []
    for v in range(2, n + 1):
        u = rng.randint(1, v - 1)
        edges.append((u, v, rng.choice([-1, 1]), rng.choice([-1, 1])))
    emit(n, edges)
elif seed == 5:
    # multi-edges between the same pair, mixed signs
    n = rng.randint(2, 10)
    m = rng.randint(1, 40)
    edges = []
    for _ in range(m):
        a, b = rng.sample(range(1, n + 1), 2)
        edges.append((a, b, rng.choice([-1, 1]), rng.choice([-1, 1])))
    emit(n, edges)
elif seed == 6:
    # many isolated vertices around a small solvable core
    n = 1000
    edges = solvable_edges(50, 100, rng)
    emit(n, edges)
else:
    n = rng.randint(2, 2000)
    m = rng.randint(1, 4000)
    if seed % 2 == 0:
        emit(n, solvable_edges(n, m, rng))
    else:
        edges = []
        for _ in range(m):
            a, b = rng.sample(range(1, n + 1), 2)
            edges.append((a, b, rng.choice([-1, 1]), rng.choice([-1, 1])))
        emit(n, edges)
