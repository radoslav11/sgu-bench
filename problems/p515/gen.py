"""Tests for p515 (Recover path): n, m <= 1e5, t <= 1e4.

A valid instance must admit a shortest path that visits all k important
cities. We build a light "backbone" path of unit-weight edges; the backbone
is itself a shortest path between its endpoints because every extra edge is
made heavy enough that no detour can be cheaper. The important cities are a
subset of backbone nodes, so a covering shortest path is guaranteed.

Seed 1 is the minimal case (n=1 forces a trivial graph). Seeds 2 and 3 are
maximal (1e5 nodes / 1e5 edges) with different shapes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

N_MAX = 10**5
M_MAX = 10**5
T_MAX = 10**4
# The reference reconstructs the path with a recursive DAG DFS whose depth
# equals the path length, so keep the covering path well within stack limits
# while still loading Dijkstra with the full edge count.
BACKBONE_CAP = 4000


def build(n, m, k, backbone_frac):
    # Backbone is the chain 1-2-...-L with unit weights and is the only way to
    # travel between backbone nodes, so it is the shortest path between its
    # endpoints. Filler edges live entirely among the pendant nodes L+1..n and
    # never touch the backbone, so they cannot create a cheaper detour.
    edges = []
    L = max(2, min(n, m + 1, int(n * backbone_frac), BACKBONE_CAP))
    for i in range(1, L):
        edges.append((i, i + 1, 1))

    pendants = list(range(L + 1, n + 1))
    while len(edges) < m and len(pendants) >= 2:
        a = rng.choice(pendants)
        b = rng.choice(pendants)
        if a == b:
            continue
        edges.append((a, b, rng.randint(1, T_MAX)))
    if len(edges) < m:
        # Not enough pendants for extra edges; pad the backbone segment by
        # making L cover the whole graph so we have m = L-1 edges.
        edges = [(i, i + 1, 1) for i in range(1, min(n, m + 1))]
    edges = edges[:m]

    # Important cities: distinct backbone nodes (always coverable by the
    # backbone shortest path).
    backbone_nodes = list(range(1, L + 1))
    k = min(k, len(backbone_nodes))
    important = rng.sample(backbone_nodes, k)

    out = [f"{n} {len(edges)}"]
    for a, b, t in edges:
        out.append(f"{a} {b} {t}")
    out.append(str(len(important)))
    out.append(" ".join(map(str, important)))
    print("\n".join(out))


if seed == 1:
    # Minimal: single road, both endpoints important.
    print("2 1")
    print("1 2 1")
    print("2")
    print("1 2")
elif seed == 2:
    build(N_MAX, M_MAX, 50000, 1.0)
elif seed == 3:
    # Maximal nodes, fewer backbone nodes, dense random filler, few important.
    build(N_MAX, M_MAX, 1000, 0.3)
elif seed == 4:
    build(1000, 5000, 20, 0.5)
elif seed == 5:
    build(2000, 2000, 100, 0.9)
else:
    n = rng.randint(3, 5000)
    m = rng.randint(n - 1, min(M_MAX, n * 3))
    k = rng.randint(1, max(1, n // 4))
    build(n, m, k, rng.choice([0.3, 0.6, 0.9, 1.0]))
