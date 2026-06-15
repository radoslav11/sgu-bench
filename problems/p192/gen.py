"""Tests for p192 (RGB): N <= 300 colored segments, integer endpoint
coordinates in [0, 32000], no two segments share more than one point
(i.e. no collinear overlap).

Seed 1 is minimal (one segment). Seed 2 is maximal: 300 wide-spanning
random segments producing close to N^2/2 intersection points. Seed 3 is
maximal with a different shape: a crossing of near-horizontal and steep
near-vertical segments. Seed 4 mixes vertical segments, shared endpoints
and touching segments. Seeds 5+ are random mid-size.
"""

import random
import sys
from math import gcd

seed = int(sys.argv[1])
rng = random.Random(seed)
MAXC = 32000


def norm(seg):
    x1, y1, x2, y2 = seg
    if (x1, y1) > (x2, y2):
        x1, y1, x2, y2 = x2, y2, x1, y1
    return x1, y1, x2, y2


def collinear_overlap(s, t):
    ax1, ay1, ax2, ay2 = norm(s)
    bx1, by1, bx2, by2 = norm(t)
    dax, day = ax2 - ax1, ay2 - ay1
    dbx, dby = bx2 - bx1, by2 - by1
    if dax * dby - day * dbx != 0:
        return False
    if dax * (by1 - ay1) - day * (bx1 - ax1) != 0:
        return False  # parallel but on different lines
    # same line: overlap iff 1D projections intersect in more than a point
    if dax != 0:
        lo, hi = max(ax1, bx1), min(ax2, bx2)
    else:
        lo, hi = max(min(ay1, ay2), min(by1, by2)), min(
            max(ay1, ay2), max(by1, by2)
        )
    return lo < hi


def add(segs, seg):
    x1, y1, x2, y2 = seg
    if (x1, y1) == (x2, y2):
        return False
    if any(collinear_overlap(seg, t) for t in segs):
        return False
    segs.append(seg)
    return True


def emit(segs):
    print(len(segs))
    for x1, y1, x2, y2 in segs:
        print(x1, y1, x2, y2, rng.choice("RGB"))


if seed == 1:
    print(1)
    print(0, 0, MAXC, MAXC, "R")
elif seed == 2:
    segs = []
    while len(segs) < 300:
        x1 = rng.randint(0, 2000)
        x2 = rng.randint(30000, MAXC)
        add(segs, (x1, rng.randint(0, MAXC), x2, rng.randint(0, MAXC)))
    emit(segs)
elif seed == 3:
    segs = []
    while len(segs) < 150:  # near-horizontal, slightly tilted
        x1, x2 = 0, MAXC
        y = rng.randint(100, MAXC - 100)
        add(segs, (x1, y, x2, y + rng.randint(-90, 90)))
    while len(segs) < 300:  # steep but not vertical
        x = rng.randint(200, MAXC - 200)
        add(segs, (x, 0, x + rng.randint(-150, 150) or 7, MAXC))
    emit(segs)
elif seed == 4:
    segs = []
    while len(segs) < 60:  # vertical segments (zero-length projection)
        x = rng.randint(0, MAXC)
        y1, y2 = sorted(rng.sample(range(0, MAXC + 1), 2))
        add(segs, (x, y1, x, y2))
    hub_pool = [
        (rng.randint(2000, 30000), rng.randint(2000, 30000)) for _ in range(8)
    ]
    while len(segs) < 240:  # fans of segments sharing endpoints
        hx, hy = rng.choice(hub_pool)
        add(segs, (hx, hy, rng.randint(0, MAXC), rng.randint(0, MAXC)))
    while len(segs) < 300:
        x1, y1 = rng.randint(0, MAXC), rng.randint(0, MAXC)
        add(segs, (x1, y1, rng.randint(0, MAXC), rng.randint(0, MAXC)))
    emit(segs)
else:
    n = rng.randint(2, 200)
    span = rng.choice([60, 1500, MAXC])
    segs = []
    while len(segs) < n:
        x1 = rng.randint(0, MAXC - 1)
        y1 = rng.randint(0, MAXC)
        x2 = min(MAXC, x1 + rng.randint(1, span))
        y2 = max(0, min(MAXC, y1 + rng.randint(-span, span)))
        add(segs, (x1, y1, x2, y2))
    emit(segs)
