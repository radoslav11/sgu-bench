"""Tests for p298 (King Berl VI): 2 <= N <= 10000, 0 <= M <= 100000,
promises X Y C meaning A[X] >= A[Y] + C, 0 <= C <= 1000.

Seed 1 is minimal (M = 0). Seed 2 is maximal and feasible: constraints
are sampled consistently with a hidden assignment. Seed 3 is maximal
with large zero-weight SCCs plus a weighted chain between them. Seed 4
is maximal but infeasible (positive cycle). Seed 5 is a tight chain
1 -> N with step 2 (span exactly 19998). Others mix random feasible /
infeasible mid-size cases and the 1-N-same-SCC special case.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, edges):
    assert len(edges) <= 100000
    print(n, len(edges))
    out = []
    for x, y, c in edges:
        assert 1 <= x <= n and 1 <= y <= n and x != y and 0 <= c <= 1000
        out.append(f"{x} {y} {c}")
    sys.stdout.write("\n".join(out) + ("\n" if out else ""))


def feasible_edges(n, m, rng, vmax=10000):
    values = [rng.randint(-vmax, vmax) for _ in range(n)]
    edges = []
    for _ in range(m):
        x, y = rng.sample(range(1, n + 1), 2)
        if values[x - 1] < values[y - 1]:
            x, y = y, x
        c = rng.randint(0, min(1000, values[x - 1] - values[y - 1]))
        edges.append((x, y, c))
    return edges


if seed == 1:
    emit(2, [])
elif seed == 2:
    emit(10000, feasible_edges(10000, 100000, rng))
elif seed == 3:
    # zero-weight SCC blobs chained with weighted edges
    n = 10000
    edges = []
    block = 100
    blocks = n // block
    for b in range(blocks):
        lo = b * block + 1
        for i in range(lo, lo + block - 1):
            edges.append((i, i + 1, 0))
            edges.append((i + 1, i, 0))
        if b + 1 < blocks:
            edges.append((lo + block, lo + block - 1, rng.randint(0, 2)))
    while len(edges) < 100000:
        b = rng.randrange(blocks)
        lo = b * block + 1
        x, y = rng.sample(range(lo, lo + block), 2)
        edges.append((x, y, 0))
    emit(n, edges[:100000])
elif seed == 4:
    # positive cycle through all vertices -> infeasible
    n = 10000
    edges = [(i % n + 1, (i + 1) % n + 1, 1) for i in range(n)]
    while len(edges) < 100000:
        x, y = rng.sample(range(1, n + 1), 2)
        edges.append((x, y, rng.randint(0, 1000)))
    emit(n, edges)
elif seed == 5:
    # tight chain: A[i+1] >= A[i] + 2, total span 19998 <= 20000
    n = 10000
    emit(n, [(i + 1, i, 2) for i in range(1, n)])
elif seed == 6:
    # chain with step 3 overflows the [-10000, 10000] window -> -1
    n = 10000
    emit(n, [(i + 1, i, 3) for i in range(1, n)])
elif seed == 7:
    # 1 and N in the same zero-weight SCC
    n = rng.randint(3, 50)
    edges = [(1, n, 0), (n, 1, 0)]
    edges += feasible_edges(n, 200, rng, vmax=20)
    emit(n, edges)
elif seed == 8:
    # small complete-ish feasible case with many duplicate pairs
    n = rng.randint(2, 10)
    edges = feasible_edges(n, 500, rng, vmax=50)
    emit(n, edges)
else:
    n = rng.randint(2, 3000)
    m = rng.randint(0, 20000)
    if rng.random() < 0.5:
        emit(n, feasible_edges(n, m, rng))
    else:
        edges = [
            (
                rng.randint(1, n),
                rng.randint(1, n),
                rng.randint(0, 1000),
            )
            for _ in range(m)
        ]
        edges = [(x, y, c) for x, y, c in edges if x != y]
        emit(n, edges)
