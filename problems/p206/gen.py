"""Tests for p206 (Roads): N <= 60 cities, M <= 400 roads, costs in [1, 1e4].

Roads 1..N-1 form a spanning tree. Seed 1 is the minimal N=2, M=1 case.
Seeds 2-3 are maximal N=60, M=400: seed 2 uniform random costs, seed 3
adversarial (expensive tree edges, cheap non-tree edges, many ties).
Later seeds are random mid sizes with mixed cost patterns.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def make(n, m, tree_cost, extra_cost, path_tree=False):
    edges = []
    for v in range(2, n + 1):
        u = v - 1 if path_tree else rng.randint(1, v - 1)
        edges.append((u, v, tree_cost()))
    for _ in range(m - (n - 1)):
        a = rng.randint(1, n)
        b = rng.randint(1, n)
        while b == a:
            b = rng.randint(1, n)
        edges.append((a, b, extra_cost()))
    print(n, m)
    for a, b, c in edges:
        print(a, b, c)


if seed == 1:
    print(2, 1)
    print(1, 2, 10000)
    sys.exit()

if seed == 2:
    make(
        60,
        400,
        lambda: rng.randint(1, 10000),
        lambda: rng.randint(1, 10000),
    )
elif seed == 3:
    make(
        60,
        400,
        lambda: rng.randint(9000, 10000),
        lambda: rng.randint(1, 30),
        path_tree=True,
    )
else:
    n = rng.randint(2, 60)
    m = rng.randint(n - 1, 400)
    if seed % 3 == 0:
        tree_cost = lambda: rng.randint(5000, 10000)
        extra_cost = lambda: rng.randint(1, 5000)
    elif seed % 3 == 1:
        tree_cost = lambda: rng.choice([10, 20, 30])
        extra_cost = lambda: rng.choice([10, 20, 30])
    else:
        tree_cost = lambda: rng.randint(1, 10000)
        extra_cost = lambda: rng.randint(1, 10000)
    make(n, m, tree_cost, extra_cost, path_tree=rng.random() < 0.5)
