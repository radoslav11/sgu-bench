"""Tests for p532 (Building Foundation): count axis-aligned rectangles whose
perimeter lies on hardened segments.

Constraints: 1 <= n <= 600, coordinates in [-1e9, 1e9], each segment axis
parallel with positive length. No two horizontal segments share a point and
no two vertical segments share a point (only perpendicular pairs may touch).

We guarantee that constraint trivially by placing every horizontal segment on
a distinct y and every vertical segment on a distinct x.

Seeds: 1 minimal, 2 and 3 maximal (different structure), rest random.
"""

import random
import sys

NMAX = 600
LO = -10**9
HI = 10**9


def emit(segs):
    out = [str(len(segs))]
    for x1, y1, x2, y2 in segs:
        out.append(f"{x1} {y1} {x2} {y2}")
    sys.stdout.write("\n".join(out) + "\n")


def grid(num_h, num_v, ys, xs):
    """Horizontals on distinct ys spanning [xs[0], xs[-1]], verticals on
    distinct xs spanning [ys[0], ys[-1]]; forms a full grid."""
    x_lo, x_hi = xs[0], xs[-1]
    y_lo, y_hi = ys[0], ys[-1]
    segs = []
    for y in ys[:num_h]:
        segs.append((x_lo, y, x_hi, y))
    for x in xs[:num_v]:
        segs.append((x, y_lo, x, y_hi))
    return segs


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    # Minimal: a single segment, no rectangle possible -> 0.
    emit([(0, 0, 5, 0)])
elif seed == 2:
    # Maximal full grid: 300 horizontals + 300 verticals = 600 segments.
    # Answer = C(300,2)^2, a huge count; also worst case for O(H^2 V).
    h = 300
    v = 300
    ys = list(range(-h // 2, -h // 2 + h))
    xs = list(range(-v // 2, -v // 2 + v))
    emit(grid(h, v, ys, xs))
elif seed == 3:
    # Maximal but skewed: many horizontals, few full verticals so the
    # inner vertical loop runs over all 600-ish verticals each pair.
    h = 599
    v = 1
    ys = list(range(1, h + 1))
    xs = [0]
    # vertical spans full y-range so it crosses every horizontal pair window
    segs = []
    for y in ys:
        segs.append((-1, y, 1, y))  # horizontal at y from x=-1..1
    segs.append((0, 1, 0, h))  # one vertical crossing all
    emit(segs)
elif seed == 4:
    # Maximal verticals, one horizontal: stresses the vertical scan count.
    # All verticals pass distinct x; only pairs of horizontals create rects,
    # so with a single horizontal answer is 0 but the scan still runs.
    v = 599
    segs = []
    xs = list(range(1, v + 1))
    for x in xs:
        segs.append((x, -1, x, 1))
    segs.append((0, 0, v + 1, 0))  # one horizontal
    emit(segs)
elif seed == 5:
    # Random partial grid: horizontals with random x-extents, verticals with
    # random y-extents, all on distinct rows/cols -> constraint satisfied.
    h = 300
    v = 300
    ys = sorted(rng.sample(range(LO, HI + 1), h))
    xs = sorted(rng.sample(range(LO, HI + 1), v))
    segs = []
    for y in ys:
        i, j = sorted(rng.sample(range(v), 2))
        segs.append((xs[i], y, xs[j], y))
    for x in xs:
        i, j = sorted(rng.sample(range(h), 2))
        segs.append((x, ys[i], x, ys[j]))
    emit(segs)
elif seed == 6:
    # Small-coordinate dense grid with many coincident extents (ties in
    # x_left / x_right boundaries).
    h = 250
    v = 250
    ys = list(range(1, h + 1))
    xs = list(range(1, v + 1))
    segs = []
    for y in ys:
        # random sub-span among small set of break points to create ties
        a = rng.randint(1, v // 2)
        b = rng.randint(v // 2 + 1, v)
        segs.append((a, y, b, y))
    for x in xs:
        a = rng.randint(1, h // 2)
        b = rng.randint(h // 2 + 1, h)
        segs.append((x, a, x, b))
    emit(segs)
else:
    # Random mid-size.
    n = rng.randint(1, NMAX)
    h = rng.randint(0, n)
    v = n - h
    h = min(h, 500)
    v = min(v, 500)
    if h + v == 0:
        h = 1
    ys = sorted(rng.sample(range(LO, HI + 1), max(h, 2)))
    xs = sorted(rng.sample(range(LO, HI + 1), max(v, 2)))
    segs = []
    for k in range(h):
        i, j = sorted(rng.sample(range(len(xs)), 2))
        segs.append((xs[i], ys[k], xs[j], ys[k]))
    for k in range(v):
        i, j = sorted(rng.sample(range(len(ys)), 2))
        segs.append((xs[k], ys[i], xs[k], ys[j]))
    emit(segs)
