"""Tests for p252 (Railway Communication): minimum path cover of a DAG
with minimal edge cost among covers of minimal size. 1 <= N <= 100,
0 <= M <= 1000, costs 0..1000, no cycles, at most one railroad per
ordered town pair.

Edges are generated as pairs (i, j) with i before j in a random
topological permutation, so the graph is always acyclic. Seed 1 is the
single isolated town, seed 2 a maximal random dense DAG, seed 3 a
maximal long chain buried under expensive shortcut edges (the cover
must follow the chain). Later seeds: the sample, no edges at all,
anti-greedy gadgets where the cheapest edge kills the maximum
matching, an all-zero-cost dense DAG and random graphs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, edges):
    perm = list(range(1, n + 1))
    rng.shuffle(perm)
    out = [f"{n} {len(edges)}"]
    lines = [
        f"{perm[u - 1]} {perm[v - 1]} {c}" for u, v, c in edges
    ]
    rng.shuffle(lines)
    out += lines
    sys.stdout.write("\n".join(out) + "\n")


def random_dag(n, m, cost_fn):
    pairs = [(i, j) for i in range(1, n + 1) for j in range(i + 1, n + 1)]
    chosen = rng.sample(pairs, min(m, len(pairs)))
    return [(u, v, cost_fn()) for u, v in chosen]


if seed == 1:
    emit(1, [])
elif seed == 2:
    emit(100, random_dag(100, 1000, lambda: rng.randint(0, 1000)))
elif seed == 3:
    n = 100
    edges = [(i, i + 1, 1000) for i in range(1, n)]
    used = {(u, v) for u, v, _ in edges}
    while len(edges) < 1000:
        i = rng.randint(1, n - 2)
        j = rng.randint(i + 2, n)
        if (i, j) not in used:
            used.add((i, j))
            edges.append((i, j, 0))
    emit(n, edges)
elif seed == 4:
    sys.stdout.write("4 4\n1 2 1\n1 3 2\n3 4 2\n2 4 2\n")
elif seed == 5:
    emit(100, [])
elif seed == 6:
    emit(100, [(i, i + 1, rng.randint(0, 1000)) for i in range(1, 100)])
elif seed == 7:
    # 25 gadgets on towns a < b < c < d: a->c is free but blocks the
    # 2-edge matching {a->d, b->c}; a greedy cover gets more paths
    n = 100
    edges = []
    for g in range(25):
        a, b, c, d = 4 * g + 1, 4 * g + 2, 4 * g + 3, 4 * g + 4
        edges.append((a, c, 0))
        edges.append((a, d, 900))
        edges.append((b, c, 900))
    emit(n, edges)
elif seed == 8:
    emit(100, random_dag(100, 1000, lambda: 0))
else:
    n = rng.randint(1, 100)
    m = rng.randint(0, min(1000, n * (n - 1) // 2))
    span = rng.choice((0, 1, 3, 1000))
    emit(n, random_dag(n, m, lambda: rng.randint(0, span)))
