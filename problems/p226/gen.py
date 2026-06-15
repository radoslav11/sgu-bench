"""Tests for p226 (Colored graph): shortest 1 -> N path in a directed
graph with 3-colored edges, adjacent path edges must differ in color.
2 <= N <= 200, 0 <= M <= N^2, duplicate edges and self-loops allowed.

Seed 1 is the minimal empty graph (answer -1). Seed 2 is a maximal
dense random graph. Seed 3 is a maximal chain test where consecutive
chain edges share a color, so the only way forward is through detour
gadgets and the path revisits vertices (long answers). Later seeds:
the two samples, a monochromatic chain (-1), a two-color trap, and
random graphs of varying density and color count.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, edges):
    out = [f"{n} {len(edges)}"]
    out += [f"{x} {y} {c}" for x, y, c in edges]
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    emit(2, [])
elif seed == 2:
    n = 200
    edges = [
        (rng.randint(1, n), rng.randint(1, n), rng.randint(1, 3))
        for _ in range(n * n)
    ]
    emit(n, edges)
elif seed == 3:
    # chain 1 -> 3 -> 5 -> ... with both chain edges around each odd
    # vertex colored 1; the only continuation is a detour through an
    # even vertex and back, so the path revisits vertices repeatedly
    n = 200
    edges = []
    spine = list(range(1, n + 1, 2))  # 1, 3, ..., 199
    for a, b in zip(spine, spine[1:]):
        edges.append((a, b, 1))
        gadget = a + 1  # even vertex between a and b
        edges.append((b, gadget, 2))
        edges.append((gadget, b, 3))
    edges.append((spine[-1], n, 1))
    # pad with useless self-loops up to the M limit
    while len(edges) < n * n:
        x = rng.randint(1, n)
        edges.append((x, x, rng.randint(1, 3)))
    rng.shuffle(edges)
    emit(n, edges)
elif seed == 4:
    emit(4, [(1, 2, 1), (2, 3, 2), (3, 4, 3), (2, 4, 1)])
elif seed == 5:
    emit(3, [(1, 2, 1), (2, 3, 1)])
elif seed == 6:
    # monochromatic chain: N is unreachable
    n = 200
    emit(n, [(i, i + 1, 2) for i in range(1, n)])
elif seed == 7:
    # the direct edge is a trap: 1->2->N shares colors, the longer
    # route 1->3->4->N is the real shortest path
    emit(
        6,
        [
            (1, 2, 1),
            (2, 6, 1),
            (1, 3, 2),
            (3, 4, 3),
            (4, 6, 2),
            (5, 5, 1),
        ],
    )
else:
    n = rng.randint(2, 200)
    m = rng.randint(0, min(n * n, rng.choice((3 * n, 20 * n, n * n))))
    colors = rng.choice(((1,), (1, 2), (1, 2, 3)))
    edges = [
        (rng.randint(1, n), rng.randint(1, n), rng.choice(colors))
        for _ in range(m)
    ]
    emit(n, edges)
