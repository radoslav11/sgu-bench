"""Tests for p300 (Train): rectilinear polyline, max train length.

N <= 4000 vertices, |coords| <= 10000, axis-parallel segments, no
self-covering. The walk alternates H/V moves; every horizontal segment
gets a unique y and every vertical segment a unique x, which rules out
collinear overlaps by construction while still producing many crossings
when the coordinate box is small. Seed 1 is N=1, seeds 2-3 are maximal
(tight box with many crossings / full coordinate range), later seeds mix
sizes, plus an intersection-free monotone staircase.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXC = 10000


def walk(n, lo, hi):
    # Alternating H/V walk; vertical segments get fresh x's, horizontal
    # segments fresh y's, so parallel segments are never collinear.
    used_x = set()
    used_y = set()
    x = rng.randint(lo, hi)
    y = rng.randint(lo, hi)
    pts = [(x, y)]
    horizontal = rng.random() < 0.5
    while len(pts) < n:
        if horizontal:
            nx = rng.randint(lo, hi)
            while nx == x or nx in used_x:
                nx = rng.randint(lo, hi)
            used_x.add(nx)
            x = nx
        else:
            ny = rng.randint(lo, hi)
            while ny == y or ny in used_y:
                ny = rng.randint(lo, hi)
            used_y.add(ny)
            y = ny
        pts.append((x, y))
        horizontal = not horizontal
    return pts


def staircase(n):
    # Monotone in both x and y: no crossings at all, answer = full length.
    x, y = -MAXC, -MAXC
    pts = [(x, y)]
    horizontal = True
    while len(pts) < n:
        step = rng.randint(1, 9)
        if horizontal:
            x = min(MAXC, x + step)
        else:
            y = min(MAXC, y + step)
        pts.append((x, y))
        horizontal = not horizontal
    return pts


if seed == 1:
    pts = [(0, 0)]  # N = 1: no segments, train length 0
elif seed == 2:
    pts = walk(4000, -1300, 1300)  # max N, dense box -> many crossings
elif seed == 3:
    pts = walk(4000, -MAXC, MAXC)  # max N, sprawling, fewer crossings
elif seed == 4:
    pts = staircase(4000)  # max N, zero intersections
elif seed == 5:
    pts = [(0, 0), (-10000, 0)]  # N = 2, single long segment
elif seed == 6:
    pts = walk(3, -50, 50)
elif seed == 7:
    pts = walk(rng.randint(4, 60), -30, 30)
elif seed == 8:
    pts = walk(rng.randint(100, 500), -300, 300)
elif seed == 9:
    pts = walk(rng.randint(500, 1500), -800, 800)
else:
    pts = walk(rng.randint(1000, 3000), -1700, 1700)

print(len(pts))
for x, y in pts:
    print(x, y)
