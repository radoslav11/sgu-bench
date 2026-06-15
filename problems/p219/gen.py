"""Tests for p219 (Synchrograph): N <= 1000 vertices, M <= 50000 arcs,
marks in [0, 1e9].

Seed 1 is the minimal case, seeds 2-3 are maximal (random dense mix and a
zero-cycle-heavy layered shape), later seeds mix zero cycles, self-loops,
all-positive graphs and long zero chains.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

N_MAX = 1000
M_MAX = 50000
W_MAX = 10**9


def emit(n, edges):
    out = [f"{n} {len(edges)}"]
    out.extend(f"{u} {v} {w}" for u, v, w in edges)
    sys.stdout.write("\n".join(out) + "\n")


def rand_w(p_zero):
    if rng.random() < p_zero:
        return 0
    return rng.choice([1, 2, rng.randint(1, W_MAX), W_MAX])


if seed == 1:
    # minimal: one vertex, one self-loop with mark 0 -> not alive
    emit(1, [(1, 1, 0)])
elif seed == 2:
    # maximal random graph, ~35% zero marks
    n, m = N_MAX, M_MAX
    edges = [
        (rng.randint(1, n), rng.randint(1, n), rand_w(0.35))
        for _ in range(m)
    ]
    emit(n, edges)
elif seed == 3:
    # maximal, structured: long zero chain through all vertices (worst-case
    # DFS depth), several large zero cycles, rest random positive arcs
    n = N_MAX
    edges = []
    perm = list(range(1, n + 1))
    rng.shuffle(perm)
    for i in range(n - 1):
        edges.append((perm[i], perm[i + 1], 0))
    for _ in range(20):
        size = rng.randint(2, 100)
        cyc = rng.sample(range(1, n + 1), size)
        for i in range(size):
            edges.append((cyc[i], cyc[(i + 1) % size], 0))
    while len(edges) < M_MAX:
        edges.append((rng.randint(1, n), rng.randint(1, n), rng.randint(1, W_MAX)))
    emit(n, edges[:M_MAX])
elif seed == 4:
    # all marks positive -> everyone alive
    n = rng.randint(2, N_MAX)
    m = rng.randint(1, 5000)
    edges = [
        (rng.randint(1, n), rng.randint(1, n), rng.randint(1, W_MAX))
        for _ in range(m)
    ]
    emit(n, edges)
elif seed == 5:
    # all marks zero, random graph -> many zero cycles
    n = rng.randint(2, 300)
    m = rng.randint(n, 3000)
    edges = [
        (rng.randint(1, n), rng.randint(1, n), 0) for _ in range(m)
    ]
    emit(n, edges)
elif seed == 6:
    # zero self-loops sprinkled on an otherwise positive random graph
    n = rng.randint(5, N_MAX)
    edges = []
    for v in rng.sample(range(1, n + 1), max(1, n // 10)):
        edges.append((v, v, 0))
    for v in rng.sample(range(1, n + 1), max(1, n // 10)):
        edges.append((v, v, rng.randint(1, W_MAX)))
    for _ in range(rng.randint(1, 8000)):
        edges.append((rng.randint(1, n), rng.randint(1, n), rng.randint(1, W_MAX)))
    rng.shuffle(edges)
    emit(n, edges)
elif seed == 7:
    # DAG of zero edges only (no zero cycles) -> all alive unless self-loop
    n = rng.randint(10, N_MAX)
    edges = []
    for _ in range(rng.randint(n, 20000)):
        u = rng.randint(1, n - 1)
        v = rng.randint(u + 1, n)
        edges.append((u, v, 0))
    emit(n, edges)
else:
    # random mid-size with varying zero density
    n = rng.randint(2, N_MAX)
    m = rng.randint(1, 20000)
    p_zero = rng.choice([0.05, 0.2, 0.5, 0.8])
    edges = [
        (rng.randint(1, n), rng.randint(1, n), rand_w(p_zero))
        for _ in range(m)
    ]
    emit(n, edges)
