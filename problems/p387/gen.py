"""Tests for p387 (Lazy Judges): n <= 50 segments, integer coords <= 100.

Feasibility (some square centered at the origin has all 4 corners
covered) is guaranteed by planting a square and covering its corners:
with its 4 sides, its diagonals, or one short segment per corner. The
remaining segment budget is filled with random clutter. Segments may
intersect but never overlap (checked exactly with integer arithmetic).

Seed 1 is a tiny diagonals-only case, seeds 2-3 use the full n = 50 with
different planted structure, seed 4 is a pure isolated-points case (the
0-dimensional branch), the rest are random mixes.
"""

import random
import sys


def rot(p, k):
    x, y = p
    for _ in range(k % 4):
        x, y = -y, x
    return (x, y)


def cross(o, a, b):
    return (a[0] - o[0]) * (b[1] - o[1]) - (a[1] - o[1]) * (b[0] - o[0])


def overlaps(s1, s2):
    (a, b), (c, d) = s1, s2
    if cross(a, b, c) != 0 or cross(a, b, d) != 0:
        return False
    ab = (b[0] - a[0], b[1] - a[1])
    tc = (c[0] - a[0]) * ab[0] + (c[1] - a[1]) * ab[1]
    td = (d[0] - a[0]) * ab[0] + (d[1] - a[1]) * ab[1]
    lo, hi = min(tc, td), max(tc, td)
    len2 = ab[0] * ab[0] + ab[1] * ab[1]
    return max(lo, 0) < min(hi, len2)


def in_range(p):
    return abs(p[0]) <= 100 and abs(p[1]) <= 100


class Builder:
    def __init__(self, rng):
        self.rng = rng
        self.segs = []

    def add(self, a, b):
        if a == b or not (in_range(a) and in_range(b)):
            return False
        if any(overlaps((a, b), s) for s in self.segs):
            return False
        self.segs.append((a, b))
        return True

    def corner_cover(self, corner):
        # Short segment with a random direction passing through `corner`.
        for _ in range(100):
            dx = self.rng.randint(-4, 4)
            dy = self.rng.randint(-4, 4)
            if (dx, dy) == (0, 0):
                continue
            s1, s2 = self.rng.randint(1, 2), self.rng.randint(1, 2)
            a = (corner[0] - s1 * dx, corner[1] - s1 * dy)
            b = (corner[0] + s2 * dx, corner[1] + s2 * dy)
            if self.add(a, b):
                return True
        return False

    def plant_square(self, corner, style):
        cs = [rot(corner, k) for k in range(4)]
        if style == "sides":
            return all(self.add(cs[k], cs[(k + 1) % 4]) for k in range(4))
        if style == "diagonals":
            return self.add(cs[0], cs[2]) and self.add(cs[1], cs[3])
        return all(self.corner_cover(c) for c in cs)

    def clutter(self, count):
        added = 0
        while added < count:
            a = (self.rng.randint(-100, 100), self.rng.randint(-100, 100))
            b = (self.rng.randint(-100, 100), self.rng.randint(-100, 100))
            if self.add(a, b):
                added += 1

    def emit(self):
        self.rng.shuffle(self.segs)
        print(len(self.segs))
        for a, b in self.segs:
            print(a[0], a[1], b[0], b[1])


seed = int(sys.argv[1])
rng = random.Random(seed)
builder = Builder(rng)

if seed == 1:
    builder.plant_square((1, 1), "diagonals")
elif seed == 2:
    assert builder.plant_square((70, 30), "sides")
    assert builder.plant_square((70, 30), "diagonals")
    builder.clutter(50 - len(builder.segs))
elif seed == 3:
    for corner in [(40, 9), (-13, 57), (88, -21)]:
        assert builder.plant_square(corner, "corners")
    builder.clutter(50 - len(builder.segs))
elif seed == 4:
    assert builder.plant_square((40, 9), "corners")
else:
    corner = (rng.randint(1, 65), rng.randint(-65, 65))
    style = rng.choice(["sides", "diagonals", "corners"])
    while not builder.plant_square(corner, style):
        builder = Builder(rng)
        corner = (rng.randint(1, 65), rng.randint(-65, 65))
    n = rng.randint(len(builder.segs), 50)
    builder.clutter(n - len(builder.segs))

builder.emit()
