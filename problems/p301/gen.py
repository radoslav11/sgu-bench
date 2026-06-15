"""Tests for p301 (Boring. Hot. Summer...): points on shortest X-Y routes.

N <= 2000 junctions, M <= 200000 distinct roads, lengths 1..10000, no
self-loops or duplicate roads. A random permutation chain keeps the
graph connected (so Y is always reachable from X), extra random edges
fill up to M. Small weight ranges create many tied shortest paths, which
stresses the DAG counting. Seeds 2-3 are maximal.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def gen(n, m, wlo, whi, x=None, y=None):
    m = min(m, n * (n - 1) // 2)
    edges = set()
    order = list(range(1, n + 1))
    rng.shuffle(order)
    for a, b in zip(order, order[1:]):
        if len(edges) >= m:
            break
        edges.add((min(a, b), max(a, b)))
    while len(edges) < m:
        a = rng.randint(1, n)
        b = rng.randint(1, n)
        if a != b:
            edges.add((min(a, b), max(a, b)))
    if x is None:
        x = rng.randint(1, n)
    if y is None:
        y = rng.randint(1, n)
    print(n, x, y)
    print(len(edges))
    lines = []
    for a, b in edges:
        if rng.random() < 0.5:
            a, b = b, a
        lines.append(f"{a} {b} {rng.randint(wlo, whi)}")
    rng.shuffle(lines)
    print("\n".join(lines))


if seed == 1:
    print(1, 1, 1)  # single junction, no roads
    print(0)
elif seed == 2:
    gen(2000, 200000, 1, 10000)  # maximal, random weights
elif seed == 3:
    gen(2000, 200000, 1, 2)  # maximal, heavy shortest-path ties
elif seed == 4:
    gen(2000, 2100, 10000, 10000)  # sparse, max weights, long distances
elif seed == 5:
    v = rng.randint(2, 30)
    gen(v, rng.randint(v - 1, v * (v - 1) // 2), 1, 5)
elif seed == 6:
    x = rng.randint(1, 50)
    gen(50, 400, 1, 10, x=x, y=x)  # X == Y
elif seed == 7:
    gen(2, 1, 1, 10000)  # two junctions, one road
elif seed == 8:
    gen(rng.randint(100, 400), rng.randint(400, 5000), 1, 1)  # unweighted-like
elif seed == 9:
    gen(rng.randint(500, 1500), rng.randint(2000, 50000), 1, 100)
else:
    gen(rng.randint(1000, 2000), rng.randint(5000, 150000), 1, 10000)
