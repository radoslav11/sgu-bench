"""Tests for p174 (Walls): 1<=M<=200000 segments, coords |.|<=10^9,
segments may touch only at endpoints and never coincide.

Walls are emitted as unit edges of a w x h integer grid (axis-parallel,
so they can only meet at shared endpoints), then scaled and offset into
the full coordinate range with random endpoint order.

Seed 1 is the single-wall case (answer 0). Seed 2 is maximal: a 199999-
edge forest plus one closing wall placed last, so the answer is 200000
(worst case for the DSU scan). Seed 3 is maximal with no cycle at all
(answer 0). Seeds 4+ are random shuffled grid-edge prefixes of various
sizes where the first cycle lands somewhere in the middle.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXC = 10**9


def grid_edges(w, h):
    edges = []
    for x in range(w):
        for y in range(h):
            if x + 1 < w:
                edges.append(((x, y), (x + 1, y)))
            if y + 1 < h:
                edges.append(((x, y), (x, y + 1)))
    return edges


class DSU:
    def __init__(self, n):
        self.p = list(range(n))

    def find(self, x):
        while self.p[x] != x:
            self.p[x] = self.p[self.p[x]]
            x = self.p[x]
        return x

    def union(self, a, b):
        ra, rb = self.find(a), self.find(b)
        if ra == rb:
            return False
        self.p[ra] = rb
        return True


def emit(edges, w, h):
    scale_x = rng.randint(1, max(1, (2 * MAXC) // max(1, w)))
    scale_y = rng.randint(1, max(1, (2 * MAXC) // max(1, h)))
    off_x = rng.randint(-MAXC, MAXC - (w - 1) * scale_x)
    off_y = rng.randint(-MAXC, MAXC - (h - 1) * scale_y)
    out = [str(len(edges))]
    for (x1, y1), (x2, y2) in edges:
        a = (off_x + x1 * scale_x, off_y + y1 * scale_y)
        b = (off_x + x2 * scale_x, off_y + y2 * scale_y)
        if rng.random() < 0.5:
            a, b = b, a
        out.append(f"{a[0]} {a[1]} {b[0]} {b[1]}")
    print("\n".join(out))


if seed == 1:
    emit([((0, 0), (1, 0))], 2, 1)
    sys.exit()

if seed in (2, 3):
    w = h = 450  # 202500 vertices, 404100 candidate edges
    target_forest = 199999 if seed == 2 else 200000
    all_edges = grid_edges(w, h)
    rng.shuffle(all_edges)
    dsu = DSU(w * h)
    vid = lambda p: p[0] * h + p[1]
    forest, closing = [], None
    for e in all_edges:
        a, b = vid(e[0]), vid(e[1])
        if dsu.find(a) == dsu.find(b):
            if closing is None:
                closing = e
        elif len(forest) < target_forest:
            dsu.union(a, b)
            forest.append(e)
        if len(forest) == target_forest and (seed == 3 or closing is not None):
            break
    rng.shuffle(forest)
    if seed == 2:
        forest.append(closing)
    emit(forest, w, h)
    sys.exit()

w = rng.randint(2, 200)
h = rng.randint(2, 200)
all_edges = grid_edges(w, h)
rng.shuffle(all_edges)
m = rng.randint(1, len(all_edges))
emit(all_edges[:m], w, h)
