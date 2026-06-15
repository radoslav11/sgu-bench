"""Tests for p345 (Revolution): convex polygon N <= 5*10^4, P <= 5*10^4
cut lines, all coordinates |.| <= 10^4 with at most 4 decimals.

Coordinates are generated on the 10^-4 grid (scaled integers) and the
polygon is built with Valtr's algorithm, so convexity is exact, never an
artifact of rounding. Seed 1 is a triangle with one cut. Seed 2 is a max
case with random chords. Seed 3 is a max case with adversarial cuts:
lines along edges, through vertices, fully outside, and tiny corner cuts,
plus collinear vertices inserted on edges. Seeds 4+ are mid-size mixes
(some polygons emitted clockwise).
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

S = 10**4  # scale: 4 decimals
BOX = 9000 * S  # keep polygon inside |coord| <= 9000 so outside lines exist


def fmt(v):
    sign = "-" if v < 0 else ""
    v = abs(v)
    return f"{sign}{v // S}.{v % S:04d}"


def convex_polygon(n):
    # Valtr's algorithm on the scaled integer grid -> exact convexity.
    while True:
        xs = sorted(rng.randint(-BOX, BOX) for _ in range(n))
        ys = sorted(rng.randint(-BOX, BOX) for _ in range(n))

        def diffs(vals):
            mn, mx = vals[0], vals[-1]
            last_top, last_bot = mn, mn
            res = []
            for v in vals[1:-1]:
                if rng.random() < 0.5:
                    res.append(v - last_top)
                    last_top = v
                else:
                    res.append(-(v - last_bot))
                    last_bot = v
            res.append(mx - last_top)
            res.append(-(mx - last_bot))
            return res

        dx = diffs(xs)
        dy = diffs(ys)
        rng.shuffle(dy)
        vec = [v for v in zip(dx, dy) if v != (0, 0)]
        vec.sort(key=lambda v: math.atan2(v[1], v[0]))
        pts = []
        x = y = 0
        for a, b in vec:
            pts.append((x, y))
            x += a
            y += b
        if len(pts) < 3:
            continue
        minx = min(p[0] for p in pts)
        miny = min(p[1] for p in pts)
        maxx = max(p[0] for p in pts)
        maxy = max(p[1] for p in pts)
        offx = rng.randint(-BOX - minx, BOX - maxx)
        offy = rng.randint(-BOX - miny, BOX - maxy)
        pts = [(p[0] + offx, p[1] + offy) for p in pts]
        # Drop exact duplicates of consecutive points just in case.
        out = [p for i, p in enumerate(pts) if p != pts[i - 1]]
        if len(out) >= 3:
            return out


def add_collinear(pts, limit):
    # Split edges at integer midpoints to create collinear vertex runs.
    out = []
    n = len(pts)
    added = 0
    for i in range(n):
        out.append(pts[i])
        if added >= limit:
            continue
        nx, ny = pts[(i + 1) % n]
        dx, dy = nx - pts[i][0], ny - pts[i][1]
        if dx % 2 == 0 and dy % 2 == 0 and (dx or dy):
            out.append((pts[i][0] + dx // 2, pts[i][1] + dy // 2))
            added += 1
    return out


def make_query(pts, mode):
    n = len(pts)
    if mode == "random":
        while True:
            a = (rng.randint(-S * 10**4, S * 10**4), rng.randint(-S * 10**4, S * 10**4))
            b = (rng.randint(-S * 10**4, S * 10**4), rng.randint(-S * 10**4, S * 10**4))
            if a != b:
                return a, b
    if mode == "chord":
        i = rng.randrange(n)
        j = (i + rng.randint(1, n - 1)) % n
        if pts[i] == pts[j]:
            return make_query(pts, "random")
        return pts[i], pts[j]
    if mode == "edge":
        i = rng.randrange(n)
        return pts[i], pts[(i + 1) % n]
    if mode == "tangent":
        i = rng.randrange(n)
        while True:
            b = (rng.randint(-S * 10**4, S * 10**4), rng.randint(-S * 10**4, S * 10**4))
            if b != pts[i]:
                return pts[i], b
    if mode == "outside":
        c = rng.choice([-1, 1]) * rng.randint(9500 * S, 10**4 * S)
        if rng.random() < 0.5:
            return (c, -10**4 * S), (c, 10**4 * S)
        return (-10**4 * S, c), (10**4 * S, c)
    if mode == "corner":
        # Line cutting very close to a vertex.
        i = rng.randrange(n)
        x, y = pts[i]
        d = rng.randint(1, 50)
        return (x - d, y + d), (x + d, y + d)
    raise ValueError(mode)


def emit(n_target, p_count, modes, clockwise=False, collinear=0):
    pts = convex_polygon(n_target)
    if collinear:
        pts = add_collinear(pts, collinear)
    if clockwise:
        pts = pts[::-1]
    print(len(pts))
    for x, y in pts:
        print(fmt(x), fmt(y))
    print(p_count)
    for _ in range(p_count):
        a, b = make_query(pts, rng.choice(modes))
        print(fmt(a[0]), fmt(a[1]), fmt(b[0]), fmt(b[1]))


if seed == 1:
    print(3)
    print("0 0")
    print("10 0")
    print("0 10")
    print(1)
    print("5 -100 5 100")
elif seed == 2:
    emit(50000, 50000, ["random", "chord"])
elif seed == 3:
    emit(49000, 50000, ["edge", "tangent", "outside", "corner", "chord"],
         collinear=1000)
elif seed == 4:
    emit(50000, 50000, ["outside", "corner", "random"], clockwise=True)
elif seed == 5:
    # Sample from the statement (square with a collinear triple).
    print(5)
    for row in ["0 0", "0 5", "0 10", "10 10", "10 0"]:
        print(row)
    print(4)
    for row in ["0 0 10 10", "9 10 10 9", "10 -1 12 11", "10 10 0 5"]:
        print(row)
else:
    emit(rng.randint(3, 2000), rng.randint(1, 2000),
         ["random", "chord", "edge", "tangent", "outside", "corner"],
         clockwise=rng.random() < 0.5,
         collinear=rng.randint(0, 30))
