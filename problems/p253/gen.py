"""Tests for p253 (Theodore Roosevelt): count explosions inside a convex
polygon, answer YES if count >= K.

3 <= N <= 1e5 polygon vertices in CCW order, 0 <= K <= M <= 1e5 query
points, |coords| <= 1e9. Seed 1 is minimal, seeds 2-3 are maximal
(random convex polygon via Valtr / circle hull). Seeds 4-5 put queries
exactly on vertices and edges with K at the sharp threshold (small
coordinates, so any arithmetic is exact). Remaining seeds cover K=0,
M=0, thin sliver polygons and mid-size randomness.
"""

import math
import random
import sys

LIM = 10**9


def valtr_polygon(rng, n, span):
    xs = sorted(rng.randint(-span, span) for _ in range(n))
    ys = sorted(rng.randint(-span, span) for _ in range(n))

    def diffs(vals):
        mn, mx = vals[0], vals[-1]
        last_top, last_bot = mn, mn
        out = []
        for v in vals[1:-1]:
            if rng.random() < 0.5:
                out.append(v - last_top)
                last_top = v
            else:
                out.append(last_bot - v)
                last_bot = v
        out.append(mx - last_top)
        out.append(last_bot - mx)
        return out

    vx = diffs(xs)
    vy = diffs(ys)
    rng.shuffle(vy)
    vecs = sorted(zip(vx, vy), key=lambda v: math.atan2(v[1], v[0]))

    merged = []
    for dx, dy in vecs:
        if dx == 0 and dy == 0:
            continue
        if merged:
            px, py = merged[-1]
            if px * dy - py * dx == 0 and px * dx + py * dy > 0:
                merged[-1] = (px + dx, py + dy)
                continue
        merged.append((dx, dy))

    pts = []
    x = y = 0
    for dx, dy in merged:
        pts.append((x, y))
        x += dx
        y += dy
    minx = min(p[0] for p in pts)
    miny = min(p[1] for p in pts)
    return [(p[0] - minx - span, p[1] - miny - span) for p in pts]


def circle_hull(rng, n, radius):
    raw = []
    for i in range(n * 2):
        a = 2 * math.pi * i / (n * 2)
        raw.append((round(radius * math.cos(a)), round(radius * math.sin(a))))
    raw = sorted(set(raw))

    def half(points):
        h = []
        for p in points:
            while len(h) >= 2 and (
                (h[-1][0] - h[-2][0]) * (p[1] - h[-2][1])
                - (h[-1][1] - h[-2][1]) * (p[0] - h[-2][0])
            ) <= 0:
                h.pop()
            h.append(p)
        return h

    lower = half(raw)
    upper = half(raw[::-1])
    return lower[:-1] + upper[:-1]


def emit(n, m, k, poly, queries):
    out = [f"{n} {m} {k}"]
    out += [f"{x} {y}" for x, y in poly]
    out += [f"{x} {y}" for x, y in queries]
    sys.stdout.write("\n".join(out) + "\n")


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit(3, 1, 1, [(0, 0), (1, 0), (0, 1)], [(0, 0)])
elif seed == 2:
    poly = valtr_polygon(rng, 100000, LIM)
    m = 100000
    queries = [
        (rng.randint(-LIM, LIM), rng.randint(-LIM, LIM)) for _ in range(m)
    ]
    emit(len(poly), m, m // 2, poly, queries)
elif seed == 3:
    radius = LIM - 10
    poly = circle_hull(rng, 50000, radius)
    scale = (radius + 10) / radius
    m = 100000
    queries = []
    inside = 0
    for _ in range(m):
        vx, vy = rng.choice(poly)
        if rng.random() < 0.5:
            queries.append((vx // 2, vy // 2))
            inside += 1
        else:
            queries.append((round(vx * scale), round(vy * scale)))
    emit(len(poly), m, inside, poly, queries)
elif seed in (4, 5):
    L = 10**6
    poly = [(0, 0), (L, 0), (L, L), (0, L)]
    queries = []
    inside = 0
    for _ in range(2000):
        kind = rng.randrange(5)
        if kind == 0:
            queries.append(rng.choice(poly))
            inside += 1
        elif kind == 1:
            t = rng.randint(0, L)
            side = rng.randrange(4)
            q = [(t, 0), (L, t), (t, L), (0, t)][side]
            queries.append(q)
            inside += 1
        elif kind == 2:
            t = rng.randint(-1, L + 1)
            side = rng.randrange(4)
            q = [(t, -1), (L + 1, t), (t, L + 1), (-1, t)][side]
            queries.append(q)
        elif kind == 3:
            queries.append((rng.randint(1, L - 1), rng.randint(1, L - 1)))
            inside += 1
        else:
            queries.append(
                (rng.choice([-1, 1]) * LIM, rng.choice([-1, 1]) * LIM)
            )
    k = inside if seed == 4 else inside + 1
    emit(4, len(queries), k, poly, queries)
elif seed == 6:
    poly = [(-LIM, -1), (LIM, 0), (-LIM + 1, 1)]
    m = 50000
    queries = [
        (rng.randint(-LIM, LIM), rng.randint(-5, 5)) for _ in range(m)
    ]
    emit(3, m, 0, poly, queries)
elif seed == 7:
    poly = valtr_polygon(rng, rng.randint(500, 2000), 10**6)
    m = rng.randint(500, 2000)
    queries = [
        (rng.randint(-(10**6), 10**6), rng.randint(-(10**6), 10**6))
        for _ in range(m)
    ]
    emit(len(poly), m, rng.randint(0, m), poly, queries)
elif seed == 8:
    poly = valtr_polygon(rng, 30, 100)
    emit(len(poly), 0, 0, poly, [])
else:
    poly = valtr_polygon(rng, rng.randint(3, 50), 10)
    m = 1000
    queries = [
        (rng.randint(-12, 12), rng.randint(-12, 12)) for _ in range(m)
    ]
    emit(len(poly), m, rng.randint(0, m), poly, queries)
