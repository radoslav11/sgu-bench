"""Tests for p349 (Wolves and Sheep): N, M <= 1e5 segments with
-1000 <= X <= 1000, 1 <= Y <= 1000; min rays from origin hitting all
wolves and no sheep.

Angular structure is built from primitive lattice directions (x, y) in a
thin band near the square boundary; distinct primitive directions differ
by >= ~5e-7 radians, far above the reference's 1e-9 epsilon, so wolf and
sheep sectors built from consecutive sorted directions never touch.

Seed 1 is the empty instance "0 0". Seed 2 is maximal with alternating
wolf/sheep sectors (large positive answer, duplicates per sector). Seed 3
is maximal fully random (overlaps everywhere, answer "No solution").
Seed 4 is 1e5 wolves and no sheep, seed 5 the reverse. Seed 6 is 2e5
degenerate point animals in separated angular halves. Later seeds are
random small/mid mixes plus the statement samples.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXC = 1000
MAXN = 100000


def directions_sorted():
    # Primitive directions (x, y) with y in 970..1000, sorted by angle.
    dirs = []
    for y in range(970, 1001):
        for x in range(-MAXC, MAXC + 1):
            if math.gcd(abs(x), y) == 1:
                dirs.append((x, y))
    dirs.sort(key=lambda p: math.atan2(p[1], p[0]))
    return dirs


def out(wolves, sheep):
    print(len(wolves), len(sheep))
    rows = ["%d %d %d %d" % s for s in wolves + sheep]
    if rows:
        print("\n".join(rows))


if seed == 1:
    out([], [])
elif seed == 2:
    dirs = directions_sorted()
    wolves, sheep = [], []
    i = 0
    # consume 4 consecutive directions per cell: wolf sector, sheep sector
    while i + 3 < len(dirs) and len(wolves) < MAXN:
        w0, w1, s0, s1 = dirs[i], dirs[i + 1], dirs[i + 2], dirs[i + 3]
        dup = rng.randint(1, 35)
        for _ in range(dup):
            if len(wolves) < MAXN:
                wolves.append((w0[0], w0[1], w1[0], w1[1]))
            if len(sheep) < MAXN:
                sheep.append((s0[0], s0[1], s1[0], s1[1]))
        i += 4
    out(wolves, sheep)
elif seed == 3:
    wolves = []
    sheep = []
    for _ in range(MAXN):
        wolves.append((
            rng.randint(-MAXC, MAXC), rng.randint(1, MAXC),
            rng.randint(-MAXC, MAXC), rng.randint(1, MAXC),
        ))
        sheep.append((
            rng.randint(-MAXC, MAXC), rng.randint(1, MAXC),
            rng.randint(-MAXC, MAXC), rng.randint(1, MAXC),
        ))
    out(wolves, sheep)
elif seed == 4:
    wolves = []
    for _ in range(MAXN):
        x = rng.randint(-MAXC, MAXC)
        y = rng.randint(1, MAXC)
        dx = rng.randint(-8, 8)
        dy = rng.randint(-8, 8)
        x2 = max(-MAXC, min(MAXC, x + dx))
        y2 = max(1, min(MAXC, y + dy))
        wolves.append((x, y, x2, y2))
    out(wolves, [])
elif seed == 5:
    sheep = []
    for _ in range(MAXN):
        sheep.append((
            rng.randint(-MAXC, MAXC), rng.randint(1, MAXC),
            rng.randint(-MAXC, MAXC), rng.randint(1, MAXC),
        ))
    out([], sheep)
elif seed == 6:
    # Degenerate points: wolves strictly in the x < 0 half, sheep in x > 0,
    # so sectors never interact and the answer counts distinct wolf rays.
    wolves = []
    sheep = []
    for _ in range(MAXN):
        x = rng.randint(-MAXC, -1)
        y = rng.randint(1, MAXC)
        wolves.append((x, y, x, y))
        x = rng.randint(1, MAXC)
        y = rng.randint(1, MAXC)
        sheep.append((x, y, x, y))
    out(wolves, sheep)
elif seed == 7:
    out([(5, 5, 6, 7)], [(3, 5, 8, 5)])  # statement sample 1
elif seed == 8:
    out([(1, 1, 2, 3), (-5, 4, 2, 2)], [(999, 1000, 1000, 999)])  # sample 2
else:
    # Random small mixes. Wolf segment endpoints never lie on the exact
    # same ray as a sheep segment endpoint: the reference resolves such
    # boundary touches with an epsilon, so they are judgment calls we keep
    # out of the data. Interior angular overlaps remain plentiful.
    n = rng.randint(0, 60)
    m = rng.randint(0, 60)
    span = rng.choice([4, 20, MAXC])

    def norm(x, y):
        g = math.gcd(abs(x), y)
        return (x // g, y // g)

    sheep = []
    sheep_dirs = set()
    for _ in range(m):
        x1 = rng.randint(-span, span)
        y1 = rng.randint(1, span)
        if rng.random() < 0.3:
            x2, y2 = x1, y1  # degenerate point
        else:
            x2 = rng.randint(-span, span)
            y2 = rng.randint(1, span)
        sheep.append((x1, y1, x2, y2))
        sheep_dirs.add(norm(x1, y1))
        sheep_dirs.add(norm(x2, y2))

    wolves = []
    attempts = 0
    while len(wolves) < n and attempts < 10000:
        attempts += 1
        x1 = rng.randint(-span, span)
        y1 = rng.randint(1, span)
        if rng.random() < 0.3:
            x2, y2 = x1, y1
        else:
            x2 = rng.randint(-span, span)
            y2 = rng.randint(1, span)
        if norm(x1, y1) in sheep_dirs or norm(x2, y2) in sheep_dirs:
            continue
        wolves.append((x1, y1, x2, y2))
    out(wolves, sheep)
