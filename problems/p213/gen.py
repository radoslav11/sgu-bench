"""Tests for p213 (Strong Defence): connected simple graph, N <= 400,
no multi-edges or loops, S != T.

Seed 1 is minimal (N=2, single tunnel). Seed 2 is the complete graph on
400 stars (max M, distance 1). Seed 3 is maximal-N layered: wide levels
chained in sequence, dense between consecutive levels, so both L and M
are large. Seed 4 is a pure path (max L = 399). Seeds 5+ are random
connected graphs of varying density, including intra-level chords.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, edges, s, t):
    perm = list(range(1, n + 1))
    rng.shuffle(perm)
    edges = [(perm[a - 1], perm[b - 1]) for a, b in edges]
    rng.shuffle(edges)
    print(n, len(edges), perm[s - 1], perm[t - 1])
    print("\n".join(f"{a} {b}" for a, b in edges))


if seed == 1:
    print(2, 1, 1, 2)
    print(1, 2)
    sys.exit()

if seed == 2:
    n = 400
    edges = [(a, b) for a in range(1, n + 1) for b in range(a + 1, n + 1)]
    emit(n, edges, 1, n)
elif seed == 3:
    # 40 levels x 10 nodes, complete bipartite between consecutive levels
    # plus all intra-level edges
    n, w = 400, 10
    levels = [list(range(i * w + 1, (i + 1) * w + 1)) for i in range(n // w)]
    edges = []
    for la, lb in zip(levels, levels[1:]):
        edges += [(a, b) for a in la for b in lb]
    for la in levels:
        edges += [(a, b) for a in la for b in la if a < b]
    emit(n, edges, levels[0][0], levels[-1][-1])
elif seed == 4:
    n = 400
    edges = [(i, i + 1) for i in range(1, n)]
    emit(n, edges, 1, n)
else:
    n = rng.randint(2, 400)
    edges = set()
    for v in range(2, n + 1):
        u = rng.randint(1, v - 1)
        edges.add((u, v))
    extra = rng.randint(0, min(n * (n - 1) // 2 - len(edges), 5 * n))
    while extra > 0:
        a, b = rng.randint(1, n), rng.randint(1, n)
        if a == b:
            continue
        if a > b:
            a, b = b, a
        if (a, b) in edges:
            continue
        edges.add((a, b))
        extra -= 1
    s = rng.randint(1, n)
    t = rng.randint(1, n)
    while t == s:
        t = rng.randint(1, n)
    emit(n, sorted(edges), s, t)
