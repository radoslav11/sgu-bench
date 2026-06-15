"""Tests for p520 (Fire in the Country): connected graph, 2<=n<=1000,
n-1<=m<=1000, simple, capital is node 1.

Seed 1 is the minimal two-city graph. Seeds 2 and 3 are maximal: a
1000-node tree padded with extra edges up to m=1000, built from two
different spanning-tree shapes (random tree vs. a long path) so the BFS
depth and branching differ. Later seeds mix random connected graphs and
adversarial shapes (paths, stars, deep balanced layers).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, edges):
    print(n, len(edges))
    for u, v in edges:
        print(u, v)


def random_tree(n, mode):
    edges = []
    if mode == "path":
        for v in range(2, n + 1):
            edges.append((v - 1, v))
    elif mode == "star":
        for v in range(2, n + 1):
            edges.append((1, v))
    else:
        for v in range(2, n + 1):
            u = rng.randint(1, v - 1)
            edges.append((u, v))
    return edges


def pad_edges(n, edges, m_target):
    present = set()
    for u, v in edges:
        present.add((min(u, v), max(u, v)))
    attempts = 0
    while len(edges) < m_target and attempts < 50 * m_target:
        attempts += 1
        u = rng.randint(1, n)
        v = rng.randint(1, n)
        if u == v:
            continue
        key = (min(u, v), max(u, v))
        if key in present:
            continue
        present.add(key)
        edges.append((u, v))
    return edges


if seed == 1:
    emit(2, [(1, 2)])
elif seed == 2:
    n = 1000
    edges = random_tree(n, "random")
    edges = pad_edges(n, edges, 1000)
    emit(n, edges)
elif seed == 3:
    n = 1000
    edges = random_tree(n, "path")
    edges = pad_edges(n, edges, 1000)
    emit(n, edges)
elif seed == 4:
    # deep balanced layers: a path -> alternating winning/losing depth
    n = 1000
    edges = random_tree(n, "path")
    emit(n, edges)
elif seed == 5:
    n = 1000
    edges = random_tree(n, "star")
    edges = pad_edges(n, edges, 1000)
    emit(n, edges)
else:
    n = rng.randint(2, 1000)
    edges = random_tree(n, "random")
    max_m = min(1000, n * (n - 1) // 2)
    m_target = rng.randint(n - 1, max_m)
    edges = pad_edges(n, edges, m_target)
    emit(n, edges)
