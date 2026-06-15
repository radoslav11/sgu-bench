"""Tests for p333 (Random Shooting): convex target, 3 <= N <= 8,
integer vertices in [1, 99], CCW, no three collinear.

Seed 1 is a minimal tiny triangle, seed 2 a maximal octagon nearly
filling the board, seed 3 a long thin sliver (hard for the adaptive
integration). Later seeds: corner-hugging triangles and random convex
polygons of every size up to 8.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def convex_hull(points):
    pts = sorted(set(points))
    if len(pts) <= 2:
        return pts

    def half(seq):
        res = []
        for p in seq:
            while len(res) > 1 and (
                (res[-1][0] - res[-2][0]) * (p[1] - res[-2][1])
                - (res[-1][1] - res[-2][1]) * (p[0] - res[-2][0])
            ) <= 0:
                res.pop()
            res.append(p)
        return res

    lower = half(pts)
    upper = half(pts[::-1])
    return lower[:-1] + upper[:-1]


def emit(pts):
    print(len(pts))
    for x, y in pts:
        print(x, y)


if seed == 1:
    emit([(50, 50), (51, 50), (50, 51)])
elif seed == 2:
    emit([(35, 1), (65, 1), (99, 35), (99, 65), (65, 99), (35, 99), (1, 65), (1, 35)])
elif seed == 3:
    emit([(1, 1), (99, 98), (99, 99), (1, 2)])  # thin diagonal sliver
elif seed == 4:
    emit([(1, 1), (3, 1), (1, 3)])  # tiny target in a corner
elif seed == 5:
    emit([(1, 49), (99, 48), (99, 52), (1, 51)])  # thin horizontal band
else:
    target_n = 3 + (seed - 6) % 6  # cycle through sizes 3..8
    while True:
        m = rng.randint(target_n, 40)
        pts = [(rng.randint(1, 99), rng.randint(1, 99)) for _ in range(m)]
        hull = convex_hull(pts)
        if len(hull) == target_n:
            emit(hull)
            break
