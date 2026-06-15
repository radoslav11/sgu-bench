"""Tests for p277 (Heroes): dynamic convex hull, doubled area after each city.

First line holds three starting cities, then N (1 <= N <= 100000) new
cities, all points distinct, |coords| <= 1e8. Seed 2 puts all 1e5 points
in convex position (circle) in random insertion order so the hull stays
huge; seed 3 is uniform random (tiny hull, many interior updates). Later
seeds mix collinear runs, parabola points and shrinking squares.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXC = 10**8


def emit(start3, rest):
    print(*[c for p in start3 for c in p])
    print(len(rest))
    out = []
    for x, y in rest:
        out.append(f"{x} {y}")
    print("\n".join(out))


def circle_points(count, radius, used):
    pts = []
    i = 0
    while len(pts) < count:
        t = 2 * math.pi * (len(pts) + i) / (count * 1.37)
        p = (int(radius * math.cos(t)), int(radius * math.sin(t)))
        if p in used:
            i += 1
            continue
        used.add(p)
        pts.append(p)
    return pts


if seed == 1:
    emit([(0, 0), (1, 0), (0, 1)], [(5, 5)])
elif seed == 2:
    # maximal: 100000 cities in convex position, random insertion order
    used = {(0, 0), (1, 0), (0, 1)}
    pts = circle_points(100000, MAXC, used)
    rng.shuffle(pts)
    emit([(0, 0), (1, 0), (0, 1)], pts)
elif seed == 3:
    # maximal: 100000 uniform random cities over the full square
    used = set()
    pts = []
    while len(pts) < 100003:
        p = (rng.randint(-MAXC, MAXC), rng.randint(-MAXC, MAXC))
        if p in used:
            continue
        used.add(p)
        pts.append(p)
    emit(pts[:3], pts[3:])
elif seed == 4:
    # collinear start (zero area) followed by collinear additions
    n = 50000
    start = [(0, 0), (1, 1), (2, 2)]
    pts = [(i, i) for i in range(3, n)] + [(0, 1), (1, 0)]
    rng.shuffle(pts)
    emit(start, pts)
elif seed == 5:
    # parabola: every point ends up on the hull, sorted insertion order
    n = 10000
    pts = [(i, i * i) for i in range(-n // 2, n // 2) if abs(i) > 1]
    emit([(0, 0), (1, 1), (-1, 1)], pts)
elif seed == 6:
    # nested squares from inside out: hull replaced over and over
    pts = []
    used = set()
    for r in range(1, 20001):
        for p in [(-r * 5000, -r * 5000), (r * 5000, -r * 5000),
                  (r * 5000, r * 5000), (-r * 5000, r * 5000)][: 4 if r % 4 else 1]:
            if p not in used:
                used.add(p)
                pts.append(p)
    pts = pts[:60000]
    emit([(0, 0), (1, 0), (0, 1)], pts)
else:
    # random mid-size with clustered coordinates (many equal x)
    span = rng.choice([10, 1000, MAXC])
    n = rng.randint(1, min(5000, (2 * span + 1) ** 2 // 2))
    used = set()
    pts = []
    while len(pts) < n + 3:
        p = (rng.randint(-span, span), rng.randint(-span, span))
        if p in used:
            continue
        used.add(p)
        pts.append(p)
    emit(pts[:3], pts[3:])
