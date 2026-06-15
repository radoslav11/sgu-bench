"""Tests for p547 (Divide The Kingdom).

Input: line 1 n (3<=n<=200); line 2 "D1 C1 D2 C2"
(0<=D1,D2<=n-1, 1<=C1,C2<=n); line 3 n costs (1<=pi<=1000); then n-1 edges
of a tree.

Seed 1 is the minimal n=3 case. Seeds 2-3 are maximal n=200: seed 2 is a
path (deep diameters, the worst case for the O(n^3) all-pairs work) with
feasible parameters; seed 3 is a random tree at n=200. Later seeds mix
random trees / parameters (often infeasible -> -1), stars, caterpillars,
and feasibility-by-construction on paths.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def random_tree(n, rng):
    edges = []
    for v in range(2, n + 1):
        u = rng.randint(1, v - 1)
        edges.append((u, v))
    return edges


def relabel(n, edges, rng):
    perm = list(range(1, n + 1))
    rng.shuffle(perm)
    mp = {i + 1: perm[i] for i in range(n)}
    return [(mp[a], mp[b]) for a, b in edges]


def emit(n, params, costs, edges):
    print(n)
    print(*params)
    print(*costs)
    for a, b in edges:
        print(a, b)


def path_edges(n):
    return [(i, i + 1) for i in range(1, n)]


if seed == 1:
    # path of 3, King takes a single city, Queen a single city, middle gone
    emit(3, (0, 1, 0, 1), [4, 7, 5], path_edges(3))
    sys.exit()

if seed == 2:
    # maximal path; split near middle gives two long sub-paths.
    n = 200
    costs = [rng.randint(1, 1000) for _ in range(n)]
    # path 1..200, destroy city 100 -> components [1..99] diam 98 and
    # [101..200] diam 99. Use D1=98, D2=99 with generous C to be feasible.
    emit(n, (98, n, 99, n), costs, path_edges(n))
    sys.exit()

if seed == 3:
    n = 200
    costs = [rng.randint(1, 1000) for _ in range(n)]
    d1 = rng.randint(0, n - 1)
    c1 = rng.randint(1, n)
    d2 = rng.randint(0, n - 1)
    c2 = rng.randint(1, n)
    edges = relabel(n, random_tree(n, rng), rng)
    emit(n, (d1, c1, d2, c2), costs, edges)
    sys.exit()

if seed == 4:
    # star: center + leaves; diameter of any subtree is <=2.
    n = 200
    costs = [rng.randint(1, 1000) for _ in range(n)]
    edges = [(1, v) for v in range(2, n + 1)]
    # King a single leaf (D1=0), Queen a single leaf (D2=0); center destroyed
    emit(n, (0, 1, 0, 1), costs, relabel(n, edges, rng))
    sys.exit()

if seed == 5:
    # feasible-by-construction on a path: choose split, set D's to fit
    n = rng.randint(10, 200)
    costs = [rng.randint(1, 1000) for _ in range(n)]
    split = rng.randint(2, n - 1)
    left_len = split - 1 - 1  # cities 1..split-1, diameter = (split-2)
    right_len = n - split - 1  # cities split+1..n
    d1 = max(0, left_len)
    d2 = max(0, right_len)
    emit(n, (d1, n, d2, n), costs, path_edges(n))
    sys.exit()

if seed == 6:
    # caterpillar tree, random-ish parameters
    n = 200
    costs = [rng.randint(1, 1000) for _ in range(n)]
    spine = n // 2
    edges = [(i, i + 1) for i in range(1, spine)]
    for v in range(spine + 1, n + 1):
        edges.append((rng.randint(1, spine), v))
    d1 = rng.randint(0, 10)
    c1 = rng.randint(1, n)
    d2 = rng.randint(0, 10)
    c2 = rng.randint(1, n)
    emit(n, (d1, c1, d2, c2), costs, relabel(n, edges, rng))
    sys.exit()

# generic random small/mid case
n = rng.randint(3, 60)
costs = [rng.randint(1, 1000) for _ in range(n)]
d1 = rng.randint(0, n - 1)
c1 = rng.randint(1, n)
d2 = rng.randint(0, n - 1)
c2 = rng.randint(1, n)
edges = relabel(n, random_tree(n, rng), rng)
emit(n, (d1, c1, d2, c2), costs, edges)
