"""Tests for p402 (Terrorists in Berland): connected graph, capture+cut.

3 <= N <= 50; 1 <= M <= 500; weights 1..10; no multi-edges; a < b;
the graph is connected. Seed 1 is the minimal path, seeds 2-3 are the
maximal dense cases (random vs two blobs joined by a few edges), seed 4
is a tree (answer 0 via an articulation point), seed 5 a single cycle.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, edge_list):
    print(n, len(edge_list))
    for a, b, w in edge_list:
        print(a, b, w)


def random_connected(n, m, rng, wlo=1, whi=10):
    order = list(range(1, n + 1))
    rng.shuffle(order)
    used = set()
    edges = []
    for i in range(1, n):
        a, b = order[rng.randint(0, i - 1)], order[i]
        a, b = min(a, b), max(a, b)
        used.add((a, b))
        edges.append((a, b, rng.randint(wlo, whi)))
    all_pairs = [
        (a, b)
        for a in range(1, n + 1)
        for b in range(a + 1, n + 1)
        if (a, b) not in used
    ]
    rng.shuffle(all_pairs)
    for a, b in all_pairs[: max(0, m - len(edges))]:
        edges.append((a, b, rng.randint(wlo, whi)))
    rng.shuffle(edges)
    return edges


if seed == 1:
    emit(3, [(1, 2, 1), (2, 3, 10)])
elif seed == 2:
    emit(50, random_connected(50, 500, rng))
elif seed == 3:
    # two dense blobs of 22 vertices joined by a handful of cheap edges
    edges = []
    for lo, hi in [(1, 22), (23, 44)]:
        pairs = [(a, b) for a in range(lo, hi + 1) for b in range(a + 1, hi + 1)]
        rng.shuffle(pairs)
        for a, b in pairs[:230]:
            edges.append((a, b, rng.randint(5, 10)))
    for v in range(45, 51):  # attach the leftover vertices
        a = rng.randint(1, 44)
        edges.append((min(a, v), v, rng.randint(1, 10)))
    for _ in range(6):  # bridges between the blobs
        a, b = rng.randint(1, 22), rng.randint(23, 44)
        if (a, b, 1) not in edges:
            edges.append((a, b, 1))
    seen = set()
    dedup = []
    for a, b, w in edges:
        if (a, b) not in seen:
            seen.add((a, b))
            dedup.append((a, b, w))
    rng.shuffle(dedup)
    emit(50, dedup[:500])
elif seed == 4:
    emit(50, random_connected(50, 49, rng))
elif seed == 5:
    cyc = [(i, i + 1, rng.randint(1, 10)) for i in range(1, 50)]
    cyc.append((1, 50, rng.randint(1, 10)))
    emit(50, cyc)
elif seed == 6:
    # all weights equal: pure structure decides
    emit(50, random_connected(50, 500, rng, wlo=10, whi=10))
else:
    n = rng.randint(3, 50)
    mmax = min(500, n * (n - 1) // 2)
    m = rng.randint(n - 1, mmax)
    emit(n, random_connected(n, m, rng))
