"""Tests for p462 (Electrician).

n wires, each (a, b, r, p) with 1 <= a, b, r, p <= 1e9, a != b.
Seed 1 is the minimal single-wire case, seeds 2 and 3 are maximal
n = 30000 tests with different structures (a dense few-node multigraph
versus a sparse many-node graph). Later seeds mix small node counts to
force many cycles, lots of equal reliabilities to stress the tie-break,
and random graphs.
"""

import random
import sys

MAXV = 10 ** 9

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(wires):
    out = [str(len(wires))]
    for a, b, r, p in wires:
        out.append(f"{a} {b} {r} {p}")
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    emit([(1, 2, rng.randint(1, MAXV), rng.randint(1, MAXV))])
    sys.exit()

if seed == 2:
    # Maximal: many wires over a small set of nodes (dense multigraph,
    # tons of cycles), reliabilities and costs collide heavily.
    n = 30000
    nodes = 50
    wires = []
    for _ in range(n):
        a = rng.randint(1, nodes)
        b = rng.randint(1, nodes)
        while b == a:
            b = rng.randint(1, nodes)
        wires.append((a, b, rng.randint(1, 5), rng.randint(1, 10)))
    emit(wires)
    sys.exit()

if seed == 3:
    # Maximal: sparse graph over many distinct nodes, large value range.
    n = 30000
    wires = []
    for _ in range(n):
        a = rng.randint(1, MAXV)
        b = rng.randint(1, MAXV)
        while b == a:
            b = rng.randint(1, MAXV)
        wires.append((a, b, rng.randint(1, MAXV), rng.randint(1, MAXV)))
    emit(wires)
    sys.exit()

if seed == 4:
    # All wires share two nodes -> every wire after the first forms a cycle.
    n = 20000
    wires = []
    for _ in range(n):
        wires.append((1, 2, rng.randint(1, 3), rng.randint(1, MAXV)))
    emit(wires)
    sys.exit()

if seed == 5:
    # Path / tree shaped graph, no cycles at all.
    n = 15000
    wires = []
    for i in range(n):
        wires.append((i + 1, i + 2, rng.randint(1, MAXV), rng.randint(1, MAXV)))
    emit(wires)
    sys.exit()

# Random mid-size tests with controlled node pool and value spreads.
n = rng.randint(2, 5000)
nodes = rng.randint(2, max(2, n // 2))
rmax = rng.choice([1, 2, 5, 1000, MAXV])
pmax = rng.choice([1, 5, 1000, MAXV])
wires = []
for _ in range(n):
    a = rng.randint(1, nodes)
    b = rng.randint(1, nodes)
    while b == a:
        b = rng.randint(1, nodes)
    wires.append((a, b, rng.randint(1, rmax), rng.randint(1, pmax)))
emit(wires)
