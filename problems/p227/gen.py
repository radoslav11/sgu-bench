"""Tests for p227 (arcs cross points): N <= 50 arcs, each given by two
endpoints and one intermediate point (integer coords, |c| <= 1000), and
the radius of the supporting circle must not exceed 1000.

Seed 1 is a single arc, seeds 2-3 are maximal N=50 clusters (dense small
box / mixed segments + arcs), seed 4 overlapping arcs on one circle
("Infinity"), seed 5 overlapping collinear segments ("Infinity"),
seed 6 endpoint-touching arcs, the rest random mixes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def circumradius2(ax, ay, bx, by, cx, cy):
    """Squared circumradius of triangle ABC, or None when collinear."""
    d = 2 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by))
    if d == 0:
        return None
    a2 = (bx - cx) ** 2 + (by - cy) ** 2
    b2 = (ax - cx) ** 2 + (ay - cy) ** 2
    c2 = (ax - bx) ** 2 + (ay - by) ** 2
    return a2 * b2 * c2 / (d * d)


def random_arc(box):
    """Three integer points whose circle has radius <= 990 (or a segment)."""
    while True:
        if rng.random() < 0.25:
            # Collinear: a true segment, intermediate point in the middle.
            ax = rng.randint(-box, box)
            ay = rng.randint(-box, box)
            dx = rng.randint(-20, 20)
            dy = rng.randint(-20, 20)
            if dx == 0 and dy == 0:
                continue
            k = rng.randint(1, 3)
            m = rng.randint(k + 1, k + 4)
            bx, by = ax + m * dx, ay + m * dy
            cx, cy = ax + k * dx, ay + k * dy
            if max(abs(bx), abs(by), abs(cx), abs(cy)) > 1000:
                continue
            return ax, ay, bx, by, cx, cy
        pts = [(rng.randint(-box, box), rng.randint(-box, box)) for _ in range(3)]
        (ax, ay), (bx, by), (cx, cy) = pts
        if len({(ax, ay), (bx, by), (cx, cy)}) < 3:
            continue
        r2 = circumradius2(ax, ay, bx, by, cx, cy)
        if r2 is None or r2 > 990 * 990:
            continue
        return ax, ay, bx, by, cx, cy


def emit(arcs):
    print(len(arcs))
    for a in arcs:
        print(*a)


if seed == 1:
    emit([(7, 4, 7, -4, 3, 0)])
elif seed == 2:
    # Maximal: 50 arcs crammed into a small box -> many pairwise crossings.
    emit([random_arc(60) for _ in range(50)])
elif seed == 3:
    # Maximal with different structure: wide spread, larger circles.
    emit([random_arc(400) for _ in range(50)])
elif seed == 4:
    # Two arcs sharing the supporting circle x^2 + y^2 = 25 and a
    # positive-length common part -> Infinity.
    arcs = [
        (5, 0, 0, 5, 3, 4),
        (4, 3, -3, 4, 0, 5),
        (-5, 0, 0, -5, -3, -4),
    ]
    emit(arcs)
elif seed == 5:
    # Overlapping collinear segments -> Infinity.
    emit([(0, 0, 10, 0, 5, 0), (4, 0, 20, 0, 12, 0)])
elif seed == 6:
    # Touching cases: segments sharing exactly an endpoint, arcs on one
    # circle that only share an endpoint, plus a tangent-ish pair.
    arcs = [
        (0, 0, 10, 0, 4, 0),
        (10, 0, 10, 8, 10, 4),
        (5, 0, 0, 5, 3, 4),
        (0, 5, -5, 0, -3, 4),
        (-5, 0, 5, 0, 0, -5),
    ]
    emit(arcs)
else:
    n = rng.randint(5, 40)
    box = rng.choice([30, 100, 700])
    emit([random_arc(box) for _ in range(n)])
