"""Tests for p530 (Recruiting game on a friendship graph).

Constraints: 3 <= n <= 1e5, 2 <= m <= 2e5, simple undirected graph,
no repeated pairs (even reversed), ai != bi. Geniuses are nodes 1, 2, 3.

Seeds: 1 minimal, 2 and 3 maximal (different shapes), rest random/adversarial.
"""

import random
import sys

NMAX = 100000
MMAX = 200000


def emit(n, edges):
    out = [f"{n} {len(edges)}"]
    for u, v in edges:
        out.append(f"{u} {v}")
    sys.stdout.write("\n".join(out) + "\n")


def random_simple(n, m, rng):
    """Up to m distinct undirected simple edges over n nodes."""
    used = set()
    edges = []
    attempts = 0
    cap = n * (n - 1) // 2
    m = min(m, cap)
    while len(edges) < m and attempts < m * 40 + 200000:
        attempts += 1
        a = rng.randint(1, n)
        b = rng.randint(1, n)
        if a == b:
            continue
        if a > b:
            a, b = b, a
        if (a, b) in used:
            continue
        used.add((a, b))
        edges.append((a, b))
    return edges, used


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    # Minimal: n=3, m=2, geniuses 1,2,3 in a path.
    emit(3, [(1, 2), (2, 3)])
elif seed == 2:
    # Maximal: n=1e5, m=2e5, sparse-ish random graph.
    n = NMAX
    edges, _ = random_simple(n, MMAX, rng)
    emit(n, edges)
elif seed == 3:
    # Maximal, dense small n: complete-ish graph hitting m cap.
    # n=640 => cap = 640*639/2 = 204480 >= 200000.
    n = 640
    edges, _ = random_simple(n, MMAX, rng)
    emit(n, edges)
elif seed == 4:
    # Long cycle through all nodes plus the three geniuses far apart.
    n = NMAX
    edges = [(i, i + 1) for i in range(1, n)]
    edges.append((1, n))  # close the cycle (1-based, n and 1)
    # m = n here, < 2e5 for n=1e5 we need to cap... n edges = 1e5 < 2e5 ok
    emit(n, edges)
elif seed == 5:
    # Star centered at node 4, geniuses on leaves (disconnected-ish star).
    n = NMAX
    edges = [(4, i) for i in range(1, n + 1) if i != 4]
    # m = n-1 < 2e5
    emit(n, edges)
elif seed == 6:
    # Two cliques: geniuses 1,2 in one, genius 3 separate; bridge structure.
    n = 600
    edges, used = random_simple(n // 2, MMAX // 2, rng)
    # offset second half
    half = n // 2
    e2, _ = random_simple(half, MMAX // 2, rng)
    for a, b in e2:
        edges.append((a + half, b + half))
    # bridge
    edges.append((1, half + 1))
    emit(n, edges)
else:
    # Random mid-size, varied density.
    n = rng.randint(3, 3000)
    cap = n * (n - 1) // 2
    m = rng.randint(2, min(MMAX, cap))
    edges, _ = random_simple(n, m, rng)
    emit(n, edges)
