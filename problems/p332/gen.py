"""Tests for p332 (Largest Circle): convex CCW polygon, |coords| <= 10^7.

Seed 1 is a minimal triangle. Seed 2 is a centrally symmetric polygon
with ~10^5 vertices built from distinct primitive edge vectors (strictly
convex, no three collinear), scaled to span the coordinate range. Seed 3
is the convex hull of the integer-rounded circle of radius 10^7 (~50k
vertices). Later seeds: thin slivers, the full square, and random hulls.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)
LIM = 10**7


def emit(pts):
    print(len(pts))
    for x, y in pts:
        print(x, y)


def convex_hull(points):
    pts = sorted(set(points))
    if len(pts) <= 2:
        return pts

    def half(pts):
        res = []
        for p in pts:
            while len(res) > 1 and (
                (res[-1][0] - res[-2][0]) * (p[1] - res[-2][1])
                - (res[-1][1] - res[-2][1]) * (p[0] - res[-2][0])
            ) <= 0:
                res.pop()
            res.append(p)
        return res

    lower = half(pts)
    upper = half(pts[::-1])
    return lower[:-1] + upper[:-1]  # CCW


def symmetric_polygon(rng, target_edges):
    k = 1
    vecs = []
    while True:
        vecs = [
            (p, q)
            for p in range(-k, k + 1)
            for q in range(-k, k + 1)
            if (p, q) != (0, 0) and math.gcd(abs(p), abs(q)) == 1
        ]
        if len(vecs) >= target_edges:
            break
        k += 1
    half = [(p, q) for p, q in vecs if q > 0 or (q == 0 and p > 0)]
    rng.shuffle(half)
    half = half[: target_edges // 2]
    edges = half + [(-p, -q) for p, q in half]
    width = sum(abs(p) for p, q in edges) // 2
    height = sum(abs(q) for p, q in edges) // 2
    scale = max(1, min((2 * LIM - 2) // width, (2 * LIM - 2) // height))
    edges = [(p * scale, q * scale) for p, q in edges]
    edges.sort(key=lambda v: math.atan2(v[1], v[0]))
    pts = []
    x = y = 0
    for p, q in edges:
        pts.append((x, y))
        x += p
        y += q
    min_x = min(p[0] for p in pts)
    min_y = min(p[1] for p in pts)
    off_x = -LIM - min_x
    off_y = -LIM - min_y
    return [(x + off_x, y + off_y) for x, y in pts]


if seed == 1:
    emit([(0, 0), (1, 0), (0, 1)])
elif seed == 2:
    emit(symmetric_polygon(rng, 100000))
elif seed == 3:
    pts = []
    m = 300000
    for i in range(m):
        ang = 2 * math.pi * i / m
        pts.append(
            (round((LIM - 1) * math.cos(ang)), round((LIM - 1) * math.sin(ang)))
        )
    emit(convex_hull(pts))
elif seed == 4:
    emit([(-LIM, 0), (LIM, -1), (LIM, 1)])  # extremely thin sliver
elif seed == 5:
    emit([(-LIM, -LIM), (LIM, -LIM), (LIM, LIM), (-LIM, LIM)])
elif seed == 6:
    emit(symmetric_polygon(rng, 1000))
else:
    while True:
        m = rng.randint(10, 3000)
        r = rng.choice([50, 10**4, LIM - 1])
        pts = [
            (rng.randint(-r, r), rng.randint(-r, r)) for _ in range(m)
        ]
        hull = convex_hull(pts)
        if len(hull) >= 3:
            emit(hull)
            break
