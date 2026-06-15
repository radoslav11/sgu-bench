"""Tests for p312 (4-3 King): N in {3,4} polygon vertices with integer
coordinates |x|,|y| <= 100, plus a ratio string K1:...:KN, 1 <= Ki <= 100.

Seed 1 is the smallest triangle, seed 2 a maximal-coordinate convex quad
with maximal ratios, seed 3 a maximal-coordinate non-convex (dart) quad.
Later seeds are random triangles and random simple quadrilaterals (both
convex and non-convex orderings), with extreme and tied ratios mixed in.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def cross(o, a, b):
    return (a[0] - o[0]) * (b[1] - o[1]) - (a[1] - o[1]) * (b[0] - o[0])


def proper_cross(p1, p2, p3, p4):
    d1 = cross(p1, p2, p3)
    d2 = cross(p1, p2, p4)
    d3 = cross(p3, p4, p1)
    d4 = cross(p3, p4, p2)
    return ((d1 > 0) != (d2 > 0)) and d1 != 0 and d2 != 0 and \
        ((d3 > 0) != (d4 > 0)) and d3 != 0 and d4 != 0


def simple_quad(pts):
    # simple polygon, no two consecutive sides collinear, positive area
    for i in range(4):
        if cross(pts[i], pts[(i + 1) % 4], pts[(i + 2) % 4]) == 0:
            return False
    e = [(pts[i], pts[(i + 1) % 4]) for i in range(4)]
    if proper_cross(*e[0], *e[2]) or proper_cross(*e[1], *e[3]):
        return False
    return True


def random_quad(lo, hi):
    while True:
        pts = [(rng.randint(lo, hi), rng.randint(lo, hi)) for _ in range(4)]
        if len(set(pts)) != 4:
            continue
        orders = [(0, 1, 2, 3), (0, 1, 3, 2), (0, 2, 1, 3)]
        rng.shuffle(orders)
        for order in orders:
            quad = [pts[i] for i in order]
            if simple_quad(quad):
                return quad


def random_tri(lo, hi):
    while True:
        pts = [(rng.randint(lo, hi), rng.randint(lo, hi)) for _ in range(3)]
        if cross(pts[0], pts[1], pts[2]) != 0:
            return pts


def emit(pts, ks):
    print(len(pts))
    for x, y in pts:
        print(x, y)
    print(":".join(map(str, ks)))


if seed == 1:
    emit([(0, 0), (1, 0), (0, 1)], [1, 1, 1])
elif seed == 2:
    emit(
        [(-100, -100), (100, -100), (100, 100), (-100, 100)],
        [100, 99, 98, 97],
    )
elif seed == 3:
    emit(
        [(-100, -100), (0, 100), (100, -100), (0, -50)],
        [1, 100, 1, 100],
    )
elif seed == 4:
    # thin sliver triangle, very skewed ratio
    emit([(-100, 0), (100, 1), (100, 0)], [100, 1, 1])
elif seed == 5:
    emit(random_tri(-100, 100), [rng.randint(1, 100) for _ in range(3)])
elif seed == 6:
    emit(random_quad(-100, 100), [rng.randint(1, 100) for _ in range(4)])
elif seed == 7:
    # equal ratios on a square: cut points land exactly on quad corners
    emit([(0, 0), (100, 0), (100, 100), (0, 100)], [1, 1, 1, 1])
elif seed == 8:
    # clockwise order
    quad = random_quad(-100, 100)
    area2 = sum(
        cross(quad[0], quad[i], quad[i + 1]) for i in range(1, 3)
    )
    if area2 > 0:
        quad.reverse()
    emit(quad, [rng.randint(1, 100) for _ in range(4)])
elif seed == 9:
    emit(random_quad(-3, 3), [1, 1, 100, 100])
elif seed == 10:
    emit(random_tri(-2, 2), [100, 1, 100])
else:
    if rng.random() < 0.5:
        emit(random_tri(-100, 100), [rng.randint(1, 100) for _ in range(3)])
    else:
        emit(random_quad(-100, 100), [rng.randint(1, 100) for _ in range(4)])
