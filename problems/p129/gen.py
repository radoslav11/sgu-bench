"""Tests for p129 (Inheritance): length of segments inside a convex polygon.

The polygon has N <= 400 vertices (given in random order, no three
collinear), coordinates in [0, 30000]; M <= 1000 query segments. Seed 1
is the minimal triangle, seeds 2-3 are maximal (N ~ 400, M = 1000) with
different polygon sizes. Later seeds add boundary-hugging segments:
chords between vertices, segments lying on edges, touching at vertices.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LIMIT = 30000


def strict_hull(points):
    """Convex hull keeping only strict turns (no three collinear)."""
    pts = sorted(set(points))
    if len(pts) <= 2:
        return pts

    def half(seq):
        res = []
        for p in seq:
            while len(res) >= 2:
                ox, oy = res[-2]
                ax, ay = res[-1]
                if (ax - ox) * (p[1] - oy) - (ay - oy) * (p[0] - ox) <= 0:
                    res.pop()
                else:
                    break
            res.append(p)
        return res

    lower = half(pts)
    upper = half(pts[::-1])
    return lower[:-1] + upper[:-1]


def convex_polygon(target_n, cx, cy, radius):
    angles = sorted(rng.uniform(0, 2 * math.pi) for _ in range(target_n))
    raw = []
    for a in angles:
        x = int(round(cx + radius * math.cos(a)))
        y = int(round(cy + radius * math.sin(a)))
        x = max(0, min(LIMIT, x))
        y = max(0, min(LIMIT, y))
        raw.append((x, y))
    hull = strict_hull(raw)
    while len(hull) < 3:
        hull = strict_hull(
            [(rng.randint(0, LIMIT), rng.randint(0, LIMIT)) for _ in range(50)]
        )
    return hull


def random_segment(lo_x, hi_x, lo_y, hi_y):
    while True:
        a = (rng.randint(lo_x, hi_x), rng.randint(lo_y, hi_y))
        b = (rng.randint(lo_x, hi_x), rng.randint(lo_y, hi_y))
        if a != b:
            return a + b


def emit(poly, segs):
    print(len(poly))
    order = list(poly)
    rng.shuffle(order)
    for x, y in order:
        print(x, y)
    print(len(segs))
    for s in segs:
        print(*s)


if seed == 1:
    print(3)
    print(0, 0)
    print(4, 0)
    print(0, 4)
    print(2)
    print(1, 1, 1, 2)
    print(5, 5, 6, 6)
    sys.exit()

if seed == 2:
    # Maximal: huge near-circular polygon, 1000 segments all over the box.
    poly = convex_polygon(400, 15000, 15000, 14990)
    segs = [random_segment(0, LIMIT, 0, LIMIT) for _ in range(1000)]
    emit(poly, segs)
    sys.exit()

if seed == 3:
    # Maximal with different structure: smaller polygon off-centre, long
    # segments crossing it plus chords between random polygon vertices.
    poly = convex_polygon(400, 6000, 22000, 5500)
    segs = []
    for i in range(1000):
        if i % 3 == 0:
            a = rng.choice(poly)
            b = rng.choice(poly)
            if a == b:
                b = poly[(poly.index(a) + 1) % len(poly)]
            segs.append(a + b)
        else:
            segs.append(random_segment(0, LIMIT, 0, LIMIT))
    emit(poly, segs)
    sys.exit()

# Seeds 4+: mid-size polygons with adversarial boundary segments.
n_target = rng.randint(3, 120)
cx, cy = rng.randint(8000, 22000), rng.randint(8000, 22000)
radius = rng.randint(500, 7500)
poly = convex_polygon(n_target, cx, cy, radius)
k = len(poly)

m = rng.randint(2, 600)
segs = []
for _ in range(m):
    kind = rng.randrange(6)
    if kind == 0:
        # Segment exactly along a polygon edge (length inside = 0).
        i = rng.randrange(k)
        segs.append(poly[i] + poly[(i + 1) % k])
    elif kind == 1:
        # Chord between two distinct vertices.
        i, j = rng.sample(range(k), 2)
        segs.append(poly[i] + poly[j])
    elif kind == 2:
        # Segment touching the polygon at one vertex, going outward-ish.
        v = rng.choice(poly)
        out = (rng.randint(0, LIMIT), rng.randint(0, LIMIT))
        if out == v:
            out = (min(LIMIT, v[0] + 1), v[1])
        segs.append(v + out)
    elif kind == 3:
        # Short segment near the centre (likely fully inside).
        a = (cx + rng.randint(-radius // 3, radius // 3),
             cy + rng.randint(-radius // 3, radius // 3))
        b = (cx + rng.randint(-radius // 3, radius // 3),
             cy + rng.randint(-radius // 3, radius // 3))
        if a == b:
            b = (a[0] + 1, a[1])
        segs.append(a + b)
    else:
        segs.append(random_segment(0, LIMIT, 0, LIMIT))
emit(poly, segs)
