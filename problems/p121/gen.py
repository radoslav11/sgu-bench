"""Tests for p121 (Bridges painting): 1 <= N <= 100 islands, adjacency
lists (each bridge listed from both endpoints) terminated by 0.

Seed 1 is a single island with no bridges, seed 2 the maximal dense
random graph on 100 islands, seed 3 a maximal "No solution" case (33
disjoint triangles plus a leftover island), seed 4 one triangle,
seed 5 a star, seed 6 an even cycle, seed 7 a path, the rest random
sparse graphs (possibly disconnected, with degree-1 islands).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, edges):
    adj = [[] for _ in range(n + 1)]
    for u, v in edges:
        adj[u].append(v)
        adj[v].append(u)
    print(n)
    for u in range(1, n + 1):
        rng.shuffle(adj[u])
        print(" ".join(map(str, adj[u] + [0])))


if seed == 1:
    emit(1, [])
elif seed == 2:
    n = 100
    edges = [(u, v) for u in range(1, n + 1) for v in range(u + 1, n + 1)
             if rng.random() < 0.55]
    emit(n, edges)
elif seed == 3:
    n = 100
    edges = []
    for t in range(33):
        a = 3 * t + 1
        edges += [(a, a + 1), (a + 1, a + 2), (a, a + 2)]
    emit(n, edges)
elif seed == 4:
    emit(3, [(1, 2), (2, 3), (1, 3)])
elif seed == 5:
    n = 50
    emit(n, [(1, v) for v in range(2, n + 1)])
elif seed == 6:
    n = 60
    emit(n, [(i, i % n + 1) for i in range(1, n + 1)])
elif seed == 7:
    n = 100
    emit(n, [(i, i + 1) for i in range(1, n)])
else:
    n = rng.randint(2, 100)
    p = rng.choice([0.03, 0.08, 0.2])
    edges = [(u, v) for u in range(1, n + 1) for v in range(u + 1, n + 1)
             if rng.random() < p]
    emit(n, edges)
