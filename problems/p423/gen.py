"""Tests for p423 (Battle): 3 <= n <= 13, symmetric 0/1 adjacency matrix
with zero diagonal, populations are positive integers (kept <= 10^6).

Seed 1 is minimal (n=3), seed 2 is a dense maximal graph, seed 3 a
sparse maximal one; later seeds vary density, population ties and
special shapes (path, star, disconnected components).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, s, t, edges, pops):
    adj = [[0] * n for _ in range(n)]
    for u, v in edges:
        adj[u][v] = adj[v][u] = 1
    print(n, s + 1, t + 1)
    for row in adj:
        print("".join(map(str, row)))
    print(" ".join(map(str, pops)))


def random_graph(n, p):
    return [(i, j) for i in range(n) for j in range(i + 1, n)
            if rng.random() < p]


if seed == 1:
    emit(3, 0, 2, [(0, 1), (1, 2)], [5, 5, 11])
elif seed == 2:
    n = 13
    emit(n, 0, 1, random_graph(n, 0.6),
         [rng.randint(1, 10**6) for _ in range(n)])
elif seed == 3:
    n = 13
    edges = [(i, i + 1) for i in range(n - 1)]
    edges += random_graph(n, 0.12)
    s, t = rng.sample(range(n), 2)
    emit(n, s, t, edges, [rng.randint(1, 100) for _ in range(n)])
elif seed == 4:
    n = 13
    emit(n, 3, 9, random_graph(n, 0.95),
         [rng.choice([1, 1, 2, 10**6]) for _ in range(n)])
elif seed == 5:
    n = 13
    edges = [(0, i) for i in range(1, n)]
    s, t = rng.sample(range(1, n), 2)
    emit(n, s, t, edges, [rng.randint(1, 1000) for _ in range(n)])
elif seed == 6:
    n = 13
    half = n // 2
    edges = [(i, j) for i in range(half) for j in range(i + 1, half)]
    edges += [(i, j) for i in range(half, n) for j in range(i + 1, n)]
    emit(n, 0, half, edges, [rng.randint(1, 10**6) for _ in range(n)])
elif seed == 7:
    n = rng.randint(3, 8)
    emit(n, 0, n - 1, random_graph(n, 0.5),
         [rng.randint(1, 50) for _ in range(n)])
elif seed == 8:
    n = 13
    emit(n, 0, 1, random_graph(n, 0.3), [1] * n)
else:
    n = rng.randint(9, 13)
    s, t = rng.sample(range(n), 2)
    emit(n, s, t, random_graph(n, rng.uniform(0.2, 0.8)),
         [rng.randint(1, 10**6) for _ in range(n)])
