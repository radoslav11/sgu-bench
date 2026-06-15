"""Tests for p430 (Unit-distance graph): 1 <= n <= 7, simple graph.

Seed 1 is the single isolated vertex (minimum). Seed 2 is K7, the densest
graph on 7 vertices (m = 21); it is not a unit-distance graph, so the
randomized reference spends its full search budget on it. Seed 3 is the
other maximal-vertex structure: the 7-cycle, a "Yes" graph at n = 7 that
makes the solver actually converge to an embedding.

Later seeds mix graphs with well-understood verdicts -- cycles, paths,
stars, empty graphs (all unit-distance) and graphs carrying a K4 or a
K_{2,3} (never unit-distance) -- with random sparse graphs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, edges):
    edges = sorted(set(tuple(sorted(e)) for e in edges))
    print(n, len(edges))
    for u, v in edges:
        print(u, v)


def cycle(n):
    return [(i, (i + 1) % n) for i in range(n)]


def path(n):
    return [(i, i + 1) for i in range(n - 1)]


def star(n):
    return [(0, i) for i in range(1, n)]


def complete(n):
    return [(i, j) for i in range(n) for j in range(i + 1, n)]


def k4_on(verts):
    return [(verts[i], verts[j]) for i in range(4) for j in range(i + 1, 4)]


if seed == 1:
    emit(1, [])
elif seed == 2:
    emit(7, complete(7))
elif seed == 3:
    emit(7, cycle(7))
elif seed == 4:
    emit(7, star(7))
elif seed == 5:
    emit(7, path(7))
elif seed == 6:
    # K4 on {0,1,2,3} plus a path tail -> not unit-distance
    emit(7, k4_on([0, 1, 2, 3]) + [(3, 4), (4, 5), (5, 6)])
elif seed == 7:
    # K_{2,3} between {0,1} and {2,3,4} -> not unit-distance
    emit(5, [(a, b) for a in (0, 1) for b in (2, 3, 4)])
else:
    n = rng.randint(2, 7)
    pairs = [(i, j) for i in range(n) for j in range(i + 1, n)]
    p = rng.choice([0.2, 0.3, 0.45])
    edges = [e for e in pairs if rng.random() < p]
    emit(n, edges)
