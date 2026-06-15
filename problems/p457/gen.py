"""Tests for p457 (Snow in Berland): n <= 100, m <= 5000, 0 <= w <= 100.

Historical roads must form (undirectedly) one component reachable from A,
which the generators below guarantee by laying a historical backbone that
touches every junction they use before adding chords. Seed 1 is minimal
(n=2, m=0). Seeds 2 and 3 are maximal feasible graphs (n=100, m=5000,
heavy snow so the flow / number of days is in the thousands): seed 2 uses
a Hamiltonian historical cycle plus dense regular shortcuts, seed 3 a
layered structure with historical edges inside layers. Later seeds cover
infeasible demands, all-clean historical roads, an unreachable sink, a
long backbone with shortcuts, and random mid-size graphs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, a, b, edges):
    assert 2 <= n <= 100
    assert len(edges) <= 5000
    seen = set()
    for x, y, w, t in edges:
        assert 1 <= x <= n and 1 <= y <= n and x != y
        assert 0 <= w <= 100 and t in (0, 1)
        assert (x, y) not in seen
        seen.add((x, y))
    print(n, len(edges), a, b)
    for x, y, w, t in edges:
        print(x, y, w, t)


def fill_regular(n, edges, used, m, wfun):
    while len(edges) < m:
        u = rng.randint(1, n)
        v = rng.randint(1, n)
        if u == v or (u, v) in used:
            continue
        used.add((u, v))
        edges.append((u, v, wfun(), 0))


if seed == 1:
    emit(2, 1, 2, [])

elif seed == 2:
    # maximal feasible: historical Hamiltonian cycle (heavy snow) plus a
    # few lighter historical chords, then dense regular shortcuts
    n = 100
    edges = []
    for i in range(1, n):
        edges.append((i, i + 1, 100, 1))
    edges.append((n, 1, 100, 1))
    used = {(x, y) for x, y, w, t in edges}
    chords = 0
    while chords < 400:
        u = rng.randint(1, n)
        v = rng.randint(1, n)
        if u == v or (u, v) in used:
            continue
        used.add((u, v))
        edges.append((u, v, rng.randint(1, 20), 1))
        chords += 1
    fill_regular(n, edges, used, 5000, lambda: rng.randint(1, 100))
    rng.shuffle(edges)
    emit(n, 1, n, edges)

elif seed == 3:
    # maximal feasible: forward historical backbone plus forward historical
    # chords (undirected historical graph is connected, reachable from A);
    # regular edges add forward and backward shortcuts
    n = 100
    edges = []
    used = set()
    for i in range(1, n):
        edges.append((i, i + 1, 100, 1))
        used.add((i, i + 1))
    h = 0
    while h < 300:
        u = rng.randint(1, n - 1)
        v = rng.randint(u + 1, n)
        if (u, v) in used:
            continue
        used.add((u, v))
        edges.append((u, v, rng.randint(1, 30), 1))
        h += 1
    fill_regular(n, edges, used, 5000, lambda: rng.randint(0, 100))
    rng.shuffle(edges)
    emit(n, 1, n, edges)

elif seed == 4:
    # infeasible: a historical road dead-ends, demand cannot be met
    emit(3, 1, 2, [(1, 3, 5, 1), (1, 2, 3, 0)])

elif seed == 5:
    # historical backbone present but all historical snow is zero -> the
    # demand is trivially met; days come only from regular A->B capacity
    n = 40
    edges = []
    used = set()
    for i in range(1, n):
        edges.append((i, i + 1, 0, 1))
        used.add((i, i + 1))
    fill_regular(n, edges, used, 400, lambda: rng.randint(0, 100))
    rng.shuffle(edges)
    emit(n, 1, n, edges)

elif seed == 6:
    # sink B has no incoming edges at all -> 0 days
    n = 20
    edges = []
    used = set()
    for i in range(1, n):
        edges.append((i, i + 1, rng.randint(1, 100), 1))
        used.add((i, i + 1))
    fill_regular(n, edges, used, 150, lambda: rng.randint(1, 100))
    edges = [e for e in edges if e[1] != n]
    emit(n, 1, n, edges)

elif seed == 7:
    # feasible long chain: uniform-snow historical backbone (so every edge
    # clears the same number of times) plus a few regular shortcuts
    n = 100
    w = rng.randint(20, 100)
    edges = []
    used = set()
    for i in range(1, n):
        edges.append((i, i + 1, w, 1))
        used.add((i, i + 1))
    fill_regular(n, edges, used, 300, lambda: rng.randint(0, 100))
    rng.shuffle(edges)
    emit(n, 1, n, edges)

elif seed == 8:
    # small graph with a back road and cycles, mixed snow
    n = 8
    edges = []
    used = set()
    for i in range(1, n):
        edges.append((i, i + 1, rng.randint(0, 5), 1))
        used.add((i, i + 1))
    edges.append((n, 1, rng.randint(1, 5), 0))
    used.add((n, 1))
    fill_regular(n, edges, used, 30, lambda: rng.randint(0, 6))
    rng.shuffle(edges)
    emit(n, 1, n, edges)

elif seed == 9:
    # no historical roads at all; just maximize A->B paths over regular roads
    n = 50
    edges = []
    used = set()
    fill_regular(n, edges, used, 800, lambda: rng.randint(0, 100))
    rng.shuffle(edges)
    emit(n, 1, n, edges)

else:
    n = rng.randint(2, 100)
    edges = []
    used = set()
    hist = rng.randint(0, n - 1)
    for i in range(1, hist + 1):
        edges.append((i, i + 1, rng.randint(0, 100), 1))
        used.add((i, i + 1))
    m_max = min(5000, n * (n - 1))
    m = rng.randint(len(edges), m_max)
    fill_regular(n, edges, used, m, lambda: rng.randint(0, 100))
    rng.shuffle(edges)
    emit(n, 1, n, edges)
