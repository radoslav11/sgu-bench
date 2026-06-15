"""Tests for p196 (Matrix Multiplication): simple undirected graph,
2 <= N <= 10000, 1 <= M <= 100000, no loops, all edges distinct.

Seed 1 is minimal (N=2, M=1). Seed 2 is a max random graph (N=10000,
M=100000). Seed 3 is max-size but concentrated on few vertices (dense
near-clique -> huge degrees). Other seeds are random sizes, stars and
mid-density graphs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, edges):
    print(n, len(edges))
    out = []
    for u, v in edges:
        out.append(f"{u} {v}")
    print("\n".join(out))


def random_graph(n, m):
    edges = set()
    while len(edges) < m:
        u = rng.randint(1, n)
        v = rng.randint(1, n)
        if u == v:
            continue
        if u > v:
            u, v = v, u
        edges.add((u, v))
    edges = list(edges)
    rng.shuffle(edges)
    return edges


if seed == 1:
    emit(2, [(1, 2)])
elif seed == 2:
    emit(10000, random_graph(10000, 100000))
elif seed == 3:
    # Dense near-clique: all C(448,2) = 100128 pairs, keep 100000 of them.
    k = 448
    all_pairs = [(u, v) for u in range(1, k + 1) for v in range(u + 1, k + 1)]
    rng.shuffle(all_pairs)
    edges = all_pairs[:100000]
    emit(10000, edges)
elif seed == 4:
    # Star: one center of degree n-1.
    n = 10000
    emit(n, [(1, v) for v in range(2, n + 1)])
elif seed == 5:
    # Long path.
    n = 10000
    emit(n, [(v, v + 1) for v in range(1, n)])
else:
    n = rng.randint(2, 10000)
    max_m = min(100000, n * (n - 1) // 2)
    m = rng.randint(1, max_m)
    emit(n, random_graph(n, m))
