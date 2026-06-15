"""Tests for p185 (Two shortest): two edge-disjoint shortest 1->N paths.

2 <= N <= 400, simple undirected graph, weights 1..10000. Seed 1 is the
minimal N=2 single-edge case ("No solution"). Seed 2 is the complete
graph on 400 vertices with random weights, seed 3 the complete graph
with all weights 1 (huge shortest-path DAG). Later seeds build layered
graphs with many tied shortest paths, single-path and disconnected
"No solution" cases, and random sparse graphs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAX_N = 400


def emit(n, edges):
    print(n, len(edges))
    out = []
    for x, y, l in edges:
        out.append(f"{x} {y} {l}")
    print("\n".join(out))


if seed == 1:
    emit(2, [(1, 2, rng.randint(1, 10000))])
elif seed == 2:
    n = MAX_N
    edges = [(i, j, rng.randint(1, 10000))
             for i in range(1, n + 1) for j in range(i + 1, n + 1)]
    emit(n, edges)
elif seed == 3:
    n = MAX_N
    edges = [(i, j, 1)
             for i in range(1, n + 1) for j in range(i + 1, n + 1)]
    emit(n, edges)
elif seed == 4:
    # layered graph: ~100 layers of 4, all consecutive-layer edges weight 1
    n = MAX_N
    layers = [[1]]
    v = 2
    while v <= n - 1:
        size = min(4, n - v)
        layers.append(list(range(v, v + size)))
        v += size
    layers.append([n])
    edges = []
    for la, lb in zip(layers, layers[1:]):
        for a in la:
            for b in lb:
                edges.append((a, b, 1))
    emit(n, edges)
elif seed == 5:
    # one cheap chain 1..n plus random expensive chords: usually no second
    # shortest path
    n = MAX_N
    edges = [(i, i + 1, 1) for i in range(1, n)]
    pairs = set((i, i + 1) for i in range(1, n))
    while len(edges) < 3000:
        a, b = rng.sample(range(1, n + 1), 2)
        a, b = min(a, b), max(a, b)
        if (a, b) in pairs:
            continue
        pairs.add((a, b))
        edges.append((a, b, 10000))
    emit(n, edges)
elif seed == 6:
    # 1 and N in different components
    n = MAX_N
    edges = [(i, i + 1, rng.randint(1, 10000)) for i in range(1, n // 2)]
    edges += [(i, i + 1, rng.randint(1, 10000)) for i in range(n // 2 + 1, n)]
    emit(n, edges)
elif seed == 7:
    # two vertex-disjoint parallel chains of equal total length + noise
    n = MAX_N
    half = (n - 2) // 2
    chain1 = [1] + list(range(2, 2 + half)) + [n]
    chain2 = [1] + list(range(2 + half, 2 + 2 * half)) + [n]
    edges = []
    pairs = set()
    for chain in (chain1, chain2):
        for a, b in zip(chain, chain[1:]):
            edges.append((min(a, b), max(a, b), 2))
            pairs.add((min(a, b), max(a, b)))
    while len(edges) < 2000:
        a, b = rng.sample(range(1, n + 1), 2)
        a, b = min(a, b), max(a, b)
        if (a, b) in pairs:
            continue
        pairs.add((a, b))
        edges.append((a, b, rng.randint(500, 10000)))
    emit(n, edges)
else:
    n = rng.randint(2, MAX_N)
    max_m = n * (n - 1) // 2
    m = rng.randint(1, min(max_m, 5000))
    all_pairs = [(i, j) for i in range(1, n + 1) for j in range(i + 1, n + 1)]
    chosen = rng.sample(all_pairs, m)
    w_hi = rng.choice([1, 3, 10000])
    emit(n, [(a, b, rng.randint(1, w_hi)) for a, b in chosen])
