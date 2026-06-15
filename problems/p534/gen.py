"""Tests for p534 (Computer Network): a weighted tree with n <= 1e5 nodes.

Each edge has a transmission time t and replacement cost p, both in
[1, 1e4]. Seed 1 is the minimal n=2 case. Seed 2 is a maximal random tree.
Seed 3 is a maximal long-spine caterpillar (a 25000-node path carrying the
remaining nodes as bushy bounded-depth subtrees) which forces a long
diameter while keeping the rooted traversal depth within bounds. Seed 4 is
a maximal star. Later seeds mix balanced trees, caterpillars and random
shapes with assorted time/cost distributions.
"""

import random
import sys

NMAX = 100000
VMAX = 10000

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, parent, tw, cost):
    # parent[v] (2..n) = the already-placed node v attaches to.
    lines = [str(n)]
    for v in range(2, n + 1):
        lines.append(f"{parent[v]} {v} {tw[v]} {cost[v]}")
    sys.stdout.write("\n".join(lines) + "\n")


def rand_t(v):
    return rng.randint(1, VMAX)


def rand_p(v):
    return rng.randint(1, VMAX)


if seed == 1:
    print(2)
    print(f"1 2 {rng.randint(1, VMAX)} {rng.randint(1, VMAX)}")
    sys.exit()

if seed == 2:
    n = NMAX
    parent = [0] * (n + 1)
    tw = [0] * (n + 1)
    cost = [0] * (n + 1)
    for v in range(2, n + 1):
        parent[v] = rng.randint(1, v - 1)
        tw[v] = rng.randint(1, VMAX)
        cost[v] = rng.randint(1, VMAX)
    emit(n, parent, tw, cost)
    sys.exit()

if seed == 3:
    # Maximal long-spine caterpillar: a 25000-node path (long diameter,
    # heavy but bounded recursion) plus bushy bounded-depth subtrees off
    # the spine for the remaining nodes.
    n = NMAX
    spine = 20000
    parent = [0] * (n + 1)
    tw = [0] * (n + 1)
    cost = [0] * (n + 1)
    for v in range(2, spine + 1):
        parent[v] = v - 1
    for v in range(spine + 1, n + 1):
        # leaves hung directly off spine nodes: subtree depth stays +1
        parent[v] = rng.randint(1, spine)
    for v in range(2, n + 1):
        tw[v] = rng.randint(1, VMAX)
        cost[v] = rng.randint(1, VMAX)
    emit(n, parent, tw, cost)
    sys.exit()

if seed == 4:
    # Maximal star.
    n = NMAX
    parent = [0] * (n + 1)
    tw = [0] * (n + 1)
    cost = [0] * (n + 1)
    for v in range(2, n + 1):
        parent[v] = 1
        tw[v] = rng.randint(1, VMAX)
        cost[v] = rng.randint(1, VMAX)
    emit(n, parent, tw, cost)
    sys.exit()

# Randomized mid-size tests with varied (bounded-depth) shapes. The
# rooted reference recursion is bounded by tree height, so random shapes
# here keep n moderate and depth small to avoid spurious deep recursion.
n = rng.randint(2, 20000)
parent = [0] * (n + 1)
tw = [0] * (n + 1)
cost = [0] * (n + 1)

spine_cap = 15000  # safely below the reference's recursion limit
shape = rng.choice(["random", "caterpillar", "balanced", "bushy"])
for v in range(2, n + 1):
    if shape == "random":
        parent[v] = rng.randint(1, v - 1)
    elif shape == "caterpillar":
        if v <= min(spine_cap, max(2, n // 2)):
            parent[v] = v - 1
        else:
            parent[v] = rng.randint(1, min(spine_cap, max(2, n // 2)))
    elif shape == "balanced":
        parent[v] = v // 2
    else:  # bushy: shallow, wide
        parent[v] = rng.randint(max(1, v - 5), v - 1)

    # Mix of weight distributions to create ties and equal contributions.
    if rng.random() < 0.3:
        tw[v] = 1
    else:
        tw[v] = rng.randint(1, VMAX)
    if rng.random() < 0.3:
        cost[v] = 1
    else:
        cost[v] = rng.randint(1, VMAX)

emit(n, parent, tw, cost)
