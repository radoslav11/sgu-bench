"""Tests for p286 (Ancient decoration): K-regular simple graph, K even,
2 <= K <= N <= 1000, M = N*K/2 edges.

Regular graphs are built as circulant graphs (vertex i joined to i+d mod
N for K/2 distinct shifts d) with randomly relabeled vertices and
shuffled edge order, plus unions of random Hamiltonian cycles and
multi-component graphs. Seeds 2-3 are the largest tests (M = N*K/2 near
100000 edges: a 200-regular graph on 1000 vertices and a 448-regular one
on 450); the reference's recursive Euler-circuit step overflows the
default 8 MB stack past roughly 110000 edges, so M is capped at 100000
even though the statement would allow denser graphs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def circulant(n, k):
    # k even, k <= n-1; choose k/2 distinct shifts in [1, ceil(n/2)-1]
    # (avoid d = n/2 when n even, it would give half-degree)
    max_d = (n - 1) // 2
    shifts = rng.sample(range(1, max_d + 1), k // 2)
    label = list(range(1, n + 1))
    rng.shuffle(label)
    edges = []
    for d in shifts:
        for i in range(n):
            edges.append((label[i], label[(i + d) % n]))
    return n, k, edges


def cycle_union(n, k):
    # union of k/2 random Hamiltonian cycles, resampled until edge-disjoint
    while True:
        used = set()
        edges = []
        ok = True
        for _ in range(k // 2):
            perm = list(range(1, n + 1))
            rng.shuffle(perm)
            cyc = []
            for i in range(n):
                u, v = perm[i], perm[(i + 1) % n]
                key = (min(u, v), max(u, v))
                if key in used:
                    ok = False
                    break
                cyc.append((u, v))
                used.add(key)
            if not ok:
                break
            edges += cyc
        if ok:
            return n, k, edges


def emit(n, k, edges):
    rng.shuffle(edges)
    out = [f"{n} {k}"]
    out += [f"{u} {v}" if rng.random() < 0.5 else f"{v} {u}" for u, v in edges]
    print("\n".join(out))


if seed == 1:
    emit(3, 2, [(1, 2), (2, 3), (3, 1)])
elif seed == 2:
    emit(*circulant(1000, 200))
elif seed == 3:
    emit(*circulant(450, 448))
elif seed == 4:
    # disconnected: ten 4-regular components of 100 vertices
    edges = []
    label = list(range(1, 1001))
    rng.shuffle(label)
    for c in range(10):
        block = label[c * 100:(c + 1) * 100]
        shifts = rng.sample(range(1, 50), 2)
        for d in shifts:
            for i in range(100):
                edges.append((block[i], block[(i + d) % 100]))
    emit(1000, 4, edges)
elif seed == 5:
    emit(*cycle_union(1000, 6))
elif seed == 6:
    # K = N-1 (complete graph on odd N): N=29, K=28
    n = 29
    edges = [(i, j) for i in range(1, n + 1) for j in range(i + 1, n + 1)]
    emit(n, n - 1, edges)
else:
    n = rng.randint(3, 60)
    max_k = 2 * ((n - 1) // 2)
    k = 2 * rng.randint(1, max_k // 2)
    if rng.random() < 0.5:
        emit(*circulant(n, k))
    else:
        emit(*cycle_union(n, min(k, 8)))
