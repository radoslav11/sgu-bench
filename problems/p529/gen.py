"""Tests for p529 (dynamic MST under edge-weight updates).

Constraints: 2 <= n <= 40000, n-1 <= m <= 40000, weights 1..40000,
1 <= t <= 40000, simple connected graph, edge index 1..m.

Seeds: 1 minimal, 2 and 3 maximal (different graph shapes), rest random.
"""

import random
import sys

W = 40000
NMAX = 40000
MMAX = 40000
TMAX = 40000


def build(n, m, rng, dense):
    """Return list of (u, v, w) simple connected graph with m edges."""
    # Spanning tree first to guarantee connectivity.
    edges = []
    used = set()
    perm = list(range(1, n + 1))
    rng.shuffle(perm)
    for i in range(1, n):
        u = perm[i]
        v = perm[rng.randint(0, i - 1)]
        a, b = (u, v) if u < v else (v, u)
        used.add((a, b))
        edges.append((a, b, rng.randint(1, W)))

    need = m - (n - 1)
    if dense:
        # Generate candidate pairs deterministically when graph is dense-ish.
        attempts = 0
        while need > 0 and attempts < need * 50 + 100000:
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
            edges.append((a, b, rng.randint(1, W)))
            need -= 1
    else:
        while need > 0:
            a = rng.randint(1, n)
            b = rng.randint(1, n)
            if a == b:
                continue
            if a > b:
                a, b = b, a
            if (a, b) in used:
                continue
            used.add((a, b))
            edges.append((a, b, rng.randint(1, W)))
            need -= 1
    return edges


def emit(n, edges, queries):
    m = len(edges)
    out = [f"{n} {m}"]
    for u, v, w in edges:
        out.append(f"{u} {v} {w}")
    out.append(str(len(queries)))
    for e, c in queries:
        out.append(f"{e} {c}")
    sys.stdout.write("\n".join(out) + "\n")


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    # Minimal: n=2, m=1, t=1.
    emit(2, [(1, 2, 5)], [(1, 9)])
elif seed == 2:
    # Maximal: complete-ish dense graph near m cap, full t.
    n = NMAX
    m = MMAX
    edges = build(n, m, rng, dense=False)
    queries = [(rng.randint(1, m), rng.randint(1, W)) for _ in range(TMAX)]
    emit(n, edges, queries)
elif seed == 3:
    # Maximal, different shape: small n => dense graph, m cap, full t.
    n = 300  # complete graph has 300*299/2 = 44850 >= 40000 edges
    m = MMAX
    edges = build(n, m, rng, dense=True)
    queries = [(rng.randint(1, m), rng.randint(1, W)) for _ in range(TMAX)]
    emit(n, edges, queries)
elif seed == 4:
    # Path graph (m = n-1), tree only, all queries hit tree edges.
    n = NMAX
    edges = []
    for i in range(2, n + 1):
        edges.append((i - 1, i, rng.randint(1, W)))
    m = len(edges)
    queries = [(rng.randint(1, m), rng.randint(1, W)) for _ in range(TMAX)]
    emit(n, edges, queries)
elif seed == 5:
    # Repeated updates to the same few edges, many equal weights.
    n = 5000
    m = 10000
    edges = build(n, m, rng, dense=False)
    edges = [(u, v, rng.choice([1, 2, 3])) for u, v, _ in edges]
    queries = [(rng.randint(1, min(m, 20)), rng.choice([1, 2, 3]))
               for _ in range(TMAX)]
    emit(n, edges, queries)
else:
    # Random mid-size.
    n = rng.randint(2, 2000)
    maxm = min(MMAX, n * (n - 1) // 2)
    m = rng.randint(n - 1, maxm)
    edges = build(n, m, rng, dense=(maxm < 4 * m))
    t = rng.randint(1, 5000)
    queries = [(rng.randint(1, m), rng.randint(1, W)) for _ in range(t)]
    emit(n, edges, queries)
