"""Tests for p240 (Runaway): N <= 100 junctions, M <= 10000 corridors
(no multi-edges, so M <= N(N-1)/2 = 4950), H <= 10000; corridor i has
travel time T, initial heat R and heating speed P; minimize the maximal
heat R + (t + T) * P along a walk from S to an exit.

Seed 1 is the minimal 2-junction case, seed 2 the maximal complete
graph on 100 junctions with one far exit, seed 3 the same size but with
tight H and large heating speeds (mostly binding constraints), seed 4 a
guaranteed NO (H = 1, hot corridors), seed 5 starts on an exit, the
rest are random sparse graphs, some disconnected.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, h, s, exits, edges):
    print(n, len(edges), h, s, len(exits))
    for a, b, t, r, p in edges:
        print(a, b, t, r, p)
    print(*exits)


def complete_edges(n, tmax, rmax, pmax):
    return [
        (a, b, rng.randint(1, tmax), rng.randint(0, rmax), rng.randint(0, pmax))
        for a in range(1, n + 1)
        for b in range(a + 1, n + 1)
    ]


def random_edges(n, m, tmax, rmax, pmax):
    pairs = rng.sample(
        [(a, b) for a in range(1, n + 1) for b in range(a + 1, n + 1)],
        m,
    )
    return [
        (a, b, rng.randint(1, tmax), rng.randint(0, rmax), rng.randint(0, pmax))
        for a, b in pairs
    ]


if seed == 1:
    emit(2, 10, 1, [2], [(1, 2, 1, 5, 3)])
elif seed == 2:
    emit(100, 10000, 1, [100], complete_edges(100, 10000, 10000, 5))
elif seed == 3:
    emit(100, 300, 7, [93], complete_edges(100, 40, 200, 60))
elif seed == 4:
    emit(50, 1, 1, [50], random_edges(50, 600, 100, 500, 100))
elif seed == 5:
    emit(30, 100, 5, [5, 17], random_edges(30, 100, 10, 10, 10))
else:
    n = rng.randint(2, 100)
    maxm = n * (n - 1) // 2
    m = rng.randint(1, min(maxm, rng.choice([3 * n, maxm])))
    h = rng.randint(1, 10000)
    s = rng.randint(1, n)
    e = rng.randint(1, min(n, 5))
    exits = rng.sample(range(1, n + 1), e)
    emit(n, h, s, exits, random_edges(n, m, 100, 2000, 50))
