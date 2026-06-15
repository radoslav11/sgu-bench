"""Tests for p134 (Centroid): centroids of a tree with N <= 16000.

Seed 1 is the single vertex, seed 2 a maximal path (worst recursion
depth, two centroids), seed 3 a maximal star. Later seeds use random
trees with various parent-distance distributions, plus a two-centroid
balanced double broom.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

NMAX = 16000


def emit(n, edges):
    labels = list(range(1, n + 1))
    rng.shuffle(labels)
    edges = [(labels[u - 1], labels[v - 1]) for u, v in edges]
    rng.shuffle(edges)
    print(n)
    for u, v in edges:
        if rng.random() < 0.5:
            u, v = v, u
        print(u, v)


if seed == 1:
    print(1)
    sys.exit()

if seed == 2:
    n = NMAX
    emit(n, [(i, i + 1) for i in range(1, n)])
    sys.exit()

if seed == 3:
    n = NMAX
    emit(n, [(1, i) for i in range(2, n + 1)])
    sys.exit()

if seed == 4:
    # Double broom: path of two vertices, each with (NMAX-2)/2 leaves.
    n = NMAX
    half = (n - 2) // 2
    edges = [(1, 2)]
    edges += [(1, i) for i in range(3, 3 + half)]
    edges += [(2, i) for i in range(3 + half, n + 1)]
    emit(n, edges)
    sys.exit()

n = rng.randint(2, NMAX)
style = rng.randrange(3)
edges = []
for v in range(2, n + 1):
    if style == 0:
        p = rng.randint(1, v - 1)
    elif style == 1:
        p = rng.randint(max(1, v - 5), v - 1)  # path-like, deep
    else:
        p = rng.randint(max(1, v // 2), v - 1)
    edges.append((p, v))
emit(n, edges)
