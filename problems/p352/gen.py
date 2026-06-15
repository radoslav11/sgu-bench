"""Tests for p352 (Beerland Attacks): shortest-path tree + extra edges.

The N and M bounds are mangled in the statement; we use N <= 50000,
M <= 100000. The tree edges (t=1) are generated first with dist[] from
the root; every extra edge (a, b) gets weight w >= |dist[a] - dist[b]|
so that the tree really is a shortest-path tree (dist is then a feasible
potential). Seed 1 is minimal, seed 2 a max shallow random tree, seed 3
a max chain (worst recursion depth), seed 4 a max star, seed 5 a tree
with no extra edges (all answers -1), seeds 6-8 random mid-size with
ties and parallel edges.
"""

import random
import sys

MAXW = 10**5

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(2, 1)
    print(1, 2, 7, 1)
    sys.exit()

if seed == 2:
    n, m, shape, wmax = 50000, 100000, "random", MAXW
elif seed == 3:
    n, m, shape, wmax = 50000, 100000, "chain", 10
elif seed == 4:
    n, m, shape, wmax = 50000, 100000, "star", MAXW
elif seed == 5:
    n, m, shape, wmax = rng.randint(2, 1000), 0, "random", MAXW
    m = n - 1
else:
    n = rng.randint(2, 3000)
    m = rng.randint(n - 1, 4 * n)
    shape = rng.choice(["random", "chain", "star"])
    wmax = rng.choice([1, 3, MAXW])

parent = [0] * (n + 1)
dist = [0] * (n + 1)
edges = []
for v in range(2, n + 1):
    if shape == "chain":
        p = v - 1
    elif shape == "star":
        p = 1
    else:
        p = rng.randint(1, v - 1)
    w = rng.randint(1, wmax)
    parent[v] = p
    dist[v] = dist[p] + w
    edges.append((p, v, w, 1))

order = sorted(range(1, n + 1), key=lambda v: dist[v])
extra = m - (n - 1)
made = 0
while made < extra:
    i = rng.randrange(n)
    j = min(n - 1, i + rng.randint(1, 60))
    if i == j:
        continue
    a, b = order[i], order[j]
    diff = dist[b] - dist[a]
    if diff > MAXW:
        continue
    w = min(MAXW, diff + rng.randint(0, max(1, wmax // 2)))
    w = max(w, 1)
    if rng.random() < 0.5:
        a, b = b, a
    edges.append((a, b, w, 0))
    made += 1

rng.shuffle(edges)
out = [f"{n} {len(edges)}"]
for a, b, w, t in edges:
    out.append(f"{a} {b} {w} {t}")
print("\n".join(out))
