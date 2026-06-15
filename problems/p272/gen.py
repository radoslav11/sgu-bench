"""Tests for p272 (Evacuation plan): N <= 10000, M <= 100000.

Each test plants a direct A-to-B connection so the required
reachability guarantee holds, and keeps the A and B sets disjoint.

Seed 1 is minimal (two labs, one tunnel). Seed 2 is maximal: a random
sparse-diameter graph with big A and B sets, plus multi-edges to reach
M = 100000. Seed 3 is maximal with a layered "ladder" structure giving
a long shortest distance K, so paths are long and the vertex-disjoint
packing matters. Other seeds mix shapes: stars, disconnected blobs
(some A labs unable to reach B), and dense small graphs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, edges, set_a, set_b):
    out = [f"{n} {len(edges)}"]
    out += [f"{u} {v}" for u, v in edges]
    out.append(str(len(set_a)))
    out.append(" ".join(map(str, set_a)))
    out.append(str(len(set_b)))
    out.append(" ".join(map(str, set_b)))
    sys.stdout.write("\n".join(out) + "\n")


def pick_sets(n, n1, n2):
    verts = rng.sample(range(1, n + 1), n1 + n2)
    return verts[:n1], verts[n1:]


if seed == 1:
    emit(2, [(1, 2)], [1], [2])
elif seed == 2:
    n, m = 10000, 100000
    set_a, set_b = pick_sets(n, 3000, 3000)
    edges = []
    order = list(range(1, n + 1))
    rng.shuffle(order)
    for i in range(1, n):  # random spanning tree: everything connected
        edges.append((order[i], order[rng.randrange(i)]))
    while len(edges) < m:
        u, v = rng.randint(1, n), rng.randint(1, n)
        if u != v:
            edges.append((u, v))
    emit(n, edges, set_a, set_b)
elif seed == 3:
    # ladder of L layers x W columns; A on the left end, B on the right
    w, layers = 25, 400
    n = w * layers

    def vid(layer, col):
        return layer * w + col + 1

    edges = []
    for layer in range(layers - 1):
        for col in range(w):
            edges.append((vid(layer, col), vid(layer + 1, col)))
            edges.append(
                (vid(layer, col), vid(layer + 1, rng.randrange(w)))
            )
    for layer in range(layers):
        for col in range(w - 1):
            edges.append((vid(layer, col), vid(layer, col + 1)))
    while len(edges) < 100000:  # duplicate tunnels are allowed
        edges.append(rng.choice(edges[:5000]))
    set_a = [vid(0, c) for c in range(w)]
    set_b = [vid(layers - 1, c) for c in range(w)]
    emit(n, edges, set_a, set_b)
elif seed == 4:
    # several disconnected blobs; only one of them contains exits
    n = 9000
    blob = n // 3
    edges = []
    for base in (0, blob, 2 * blob):
        for i in range(2, blob + 1):
            edges.append((base + i, base + rng.randint(1, i - 1)))
        for _ in range(8000):
            u = base + rng.randint(1, blob)
            v = base + rng.randint(1, blob)
            if u != v:
                edges.append((u, v))
    set_a = rng.sample(range(1, blob + 1), 500) + rng.sample(
        range(blob + 1, 2 * blob + 1), 500
    )
    pool = [v for v in range(1, blob + 1) if v not in set(set_a)]
    set_b = rng.sample(pool, 500)
    emit(n, edges, set_a, set_b)
else:
    n = rng.randint(2, 400)
    m = rng.randint(1, 2000)
    n1 = rng.randint(1, max(1, n // 3))
    n2 = rng.randint(1, max(1, n // 3))
    set_a, set_b = pick_sets(n, n1, n2)
    edges = [(set_a[0], set_b[0])]
    while len(edges) < m:
        u, v = rng.randint(1, n), rng.randint(1, n)
        if u != v:
            edges.append((u, v))
    emit(n, edges, set_a, set_b)
