"""Tests for p124 (Broken line): closed axis-parallel simple polygon,
4 <= K <= 10000 segments, all coordinates in [-10000, 10000].

Polygons are random "skylines": columns x = 0..m-1 with alternating
heights, closed by two sides and a bottom; segments are shuffled and
endpoint order randomized. Seed 1 is the minimal square with the point
inside, seeds 2-4 maximal skylines (~10000 segments) with the point
inside a valley / on a vertical wall / outside but inside the bounding
box, seed 5 a corner vertex (BORDER), seed 6 far outside, the rest
random skylines with random points.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def skyline(m, hmax=9000):
    # heights per column, consecutive different so every step exists
    hs = []
    prev = 0
    for _ in range(m):
        h = rng.randint(1, hmax)
        while h == prev:
            h = rng.randint(1, hmax)
        hs.append(h)
        prev = h
    segs = []
    segs.append((0, 0, 0, hs[0]))                 # left wall
    for i in range(m):
        segs.append((i, hs[i], i + 1, hs[i]))     # top of column i
        if i + 1 < m:
            segs.append((i + 1, hs[i], i + 1, hs[i + 1]))
    segs.append((m, hs[-1], m, 0))                # right wall
    segs.append((m, 0, 0, 0))                     # bottom
    return hs, segs


def emit(segs, px, py):
    segs = list(segs)
    rng.shuffle(segs)
    print(len(segs))
    out = []
    for x1, y1, x2, y2 in segs:
        if rng.random() < 0.5:
            x1, y1, x2, y2 = x2, y2, x1, y1
        out.append(f"{x1} {y1} {x2} {y2}")
    out.append(f"{px} {py}")
    print("\n".join(out))


if seed == 1:
    emit([(0, 0, 0, 3), (0, 3, 3, 3), (3, 3, 3, 0), (3, 0, 0, 0)], 2, 2)
elif seed == 2:
    hs, segs = skyline(4999)
    c = rng.randrange(4999)
    emit(segs, c, rng.randint(1, hs[c] - 1) if hs[c] > 1 else 0)
elif seed == 3:
    hs, segs = skyline(4999)
    c = rng.randrange(1, 4999)
    lo, hi = sorted((hs[c - 1], hs[c]))
    emit(segs, c, rng.randint(lo, hi))
elif seed == 4:
    hs, segs = skyline(4999)
    c = max(range(4999), key=lambda i: -hs[i])
    emit(segs, c, hs[c] + 1)
elif seed == 5:
    emit([(0, 0, 0, 3), (0, 3, 3, 3), (3, 3, 3, 0), (3, 0, 0, 0)], 0, 3)
elif seed == 6:
    hs, segs = skyline(200)
    emit(segs, -9999, 9999)
else:
    m = rng.randint(2, 2000)
    hs, segs = skyline(m)
    emit(segs, rng.randint(-5, m + 5), rng.randint(-5, 9005))
