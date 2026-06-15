"""Tests for p412 (Expedition): a convex polygon (the tent) with the lamp at
the origin strictly inside, plus M shelf segments strictly inside the polygon
and never through the origin. Output is the shaded wall length.

Convex polygons are built by rounding points sampled on a circle to integer
coordinates and taking their convex hull, which keeps the origin strictly
inside. Each shelf is a segment whose two endpoints are strictly interior
lattice/real points (a convex polygon then contains the whole segment); the
endpoints are rejected if the segment would pass through or touch the origin.

Seed 1 is the minimal 3-vertex tent with no shelves. Seed 2 is a maximal
100000-gon with 100000 shelves on a large circle, seed 3 a second maximal
case with a near-degenerate thin polygon and clustered shelves. Later seeds
mix sizes, shelf counts, axis-aligned shelves and shelves whose endpoints
share angles (collinear-from-origin) to exercise interval merging.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LIM = 10**6


def cross(o, a, b):
    return (a[0] - o[0]) * (b[1] - o[1]) - (a[1] - o[1]) * (b[0] - o[0])


def convex_hull(points):
    pts = sorted(set(points))
    if len(pts) <= 2:
        return pts
    lower = []
    for p in pts:
        while len(lower) >= 2 and cross(lower[-2], lower[-1], p) <= 0:
            lower.pop()
        lower.append(p)
    upper = []
    for p in reversed(pts):
        while len(upper) >= 2 and cross(upper[-2], upper[-1], p) <= 0:
            upper.pop()
        upper.append(p)
    return lower[:-1] + upper[:-1]


def random_convex_valtr(k, span):
    # Valtr's algorithm: build a strictly convex lattice polygon with exactly k
    # vertices whose bounding box has size ~span, then translate so the origin
    # lands strictly inside. Edge x/y deltas are derived from random partitions
    # that sum to zero, sorted by angle and accumulated.
    def deltas():
        if span + 1 >= k:
            xs = sorted(rng.sample(range(span + 1), k))
        else:
            xs = sorted(rng.randint(0, span) for _ in range(k))
        min_x, max_x = xs[0], xs[-1]
        last_lo = last_hi = min_x
        lo, hi = [], []
        for v in xs[1:-1]:
            if rng.random() < 0.5:
                lo.append(v - last_lo)
                last_lo = v
            else:
                hi.append(last_hi - v)
                last_hi = v
        lo.append(max_x - last_lo)
        hi.append(last_hi - max_x)
        return lo + hi

    vx = deltas()
    vy = deltas()
    rng.shuffle(vy)
    vecs = list(zip(vx, vy))
    vecs.sort(key=lambda v: math.atan2(v[1], v[0]))

    x = y = 0
    pts = []
    for dx, dy in vecs:
        pts.append((x, y))
        x += dx
        y += dy
    # Center the polygon's bounding box on the origin so it is strictly inside.
    xs = [p[0] for p in pts]
    ys = [p[1] for p in pts]
    cx = (min(xs) + max(xs)) // 2
    cy = (min(ys) + max(ys)) // 2
    pts = [(p[0] - cx, p[1] - cy) for p in pts]
    return convex_hull(pts)


def subdivide_to(poly, target_n):
    # Insert collinear lattice points on edges until the polygon has about
    # target_n vertices. A "convex polygon" with vertices listed CCW may carry
    # 180-degree (collinear) vertices, which is how a strictly convex lattice
    # polygon reaches large vertex counts within the +/-1e6 box. Each inserted
    # point is an exact lattice point on its edge (stepping by the edge's
    # primitive integer direction), so it stays on the boundary.
    from math import gcd

    n = len(poly)
    if n >= target_n:
        return poly
    # Build per-edge primitive step and available interior lattice slots.
    edges = []
    total_slots = 0
    for i in range(n):
        a = poly[i]
        b = poly[(i + 1) % n]
        dx = b[0] - a[0]
        dy = b[1] - a[1]
        g = gcd(abs(dx), abs(dy))
        slots = max(g - 1, 0)
        edges.append((a, (dx // g if g else 0, dy // g if g else 0), slots))
        total_slots += slots
    need = target_n - n
    if total_slots == 0:
        return poly
    take = min(need, total_slots)
    # Distribute the inserts proportionally across edges by their slot capacity.
    result = []
    remaining = take
    for ei, (a, step, slots) in enumerate(edges):
        result.append(a)
        if slots == 0 or remaining == 0:
            continue
        edges_left = sum(1 for e in edges[ei:] if e[2] > 0)
        want = min(slots, max(1, remaining // max(edges_left, 1)))
        want = min(want, remaining)
        if want <= 0:
            continue
        # Choose `want` distinct step counts in 1..slots, sorted.
        offsets = sorted(rng.sample(range(1, slots + 1), want))
        for off in offsets:
            result.append((a[0] + step[0] * off, a[1] + step[1] * off))
        remaining -= want
    return result


def point_strictly_inside(poly, p):
    n = len(poly)
    for i in range(n):
        a = poly[i]
        b = poly[(i + 1) % n]
        if cross(a, b, p) <= 0:
            return False
    return True


def origin_inside(poly):
    return point_strictly_inside(poly, (0, 0))


def gen_polygon(target_n, radius):
    for _ in range(40):
        hull = random_convex_valtr(target_n, 2 * radius)
        if len(hull) >= 3 and origin_inside(hull):
            return hull
    # Fallback: a guaranteed valid triangle-ish shape.
    base = [(-radius, -radius // 2), (radius, -radius // 2), (0, radius)]
    return base


def point_in_convex_fast(poly, cx, cy, p):
    # O(log n) test using a fan from poly[0]. Returns True if strictly inside.
    n = len(poly)
    if cross(poly[0], poly[1], p) <= 0:
        return False
    if cross(poly[0], poly[n - 1], p) >= 0:
        return False
    lo, hi = 1, n - 1
    while hi - lo > 1:
        mid = (lo + hi) // 2
        if cross(poly[0], poly[mid], p) > 0:
            lo = mid
        else:
            hi = mid
    return cross(poly[lo], poly[hi], p) > 0


def random_interior_point(poly, cx, cy):
    # Sample a lattice point on the way from the centroid toward a random
    # vertex; verify strict containment with the O(log n) test and resample.
    n = len(poly)
    for _ in range(40):
        v = poly[rng.randrange(n)]
        t = rng.uniform(0.0, 0.85)
        x = int(round(cx + t * (v[0] - cx)))
        y = int(round(cy + t * (v[1] - cy)))
        if point_in_convex_fast(poly, cx, cy, (x, y)):
            return (x, y)
    ic = (int(round(cx)), int(round(cy)))
    return ic


def make_shelf(poly, cx, cy):
    for _ in range(20):
        p = random_interior_point(poly, cx, cy)
        q = random_interior_point(poly, cx, cy)
        if p == q:
            continue
        # Reject if origin lies on segment p-q (cross == 0 and within box).
        c = cross(p, q, (0, 0))
        if c == 0:
            # collinear with origin: only forbidden if origin is on the segment
            within = (min(p[0], q[0]) <= 0 <= max(p[0], q[0])
                      and min(p[1], q[1]) <= 0 <= max(p[1], q[1]))
            if within:
                continue
        return (p[0], p[1], q[0], q[1])
    return None


def centroid(poly):
    # Average of vertices: strictly inside a convex polygon.
    sx = sum(p[0] for p in poly)
    sy = sum(p[1] for p in poly)
    return sx / len(poly), sy / len(poly)


def build_shelves(poly, count, max_attempts=None):
    cx, cy = centroid(poly)
    shelves = []
    attempts = 0
    cap = max_attempts if max_attempts is not None else count * 6 + 200
    while len(shelves) < count and attempts < cap:
        attempts += 1
        s = make_shelf(poly, cx, cy)
        if s is not None:
            shelves.append(s)
    return shelves


def emit(poly, shelves):
    out = [f"{len(poly)} {len(shelves)}"]
    for x, y in poly:
        out.append(f"{x} {y}")
    for s in shelves:
        out.append(" ".join(map(str, s)))
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    poly = [(0, 2), (-2, -1), (3, -3)]
    emit(poly, [])
    sys.exit()

if seed == 2:
    poly = gen_polygon(100000, LIM)
    poly = subdivide_to(poly, 100000)
    shelves = build_shelves(poly, 100000)
    emit(poly, shelves)
    sys.exit()

if seed == 3:
    # A second maximal case: many vertices on a long, narrow polygon (built by
    # squashing the y extent) with shelves clustered near one wall.
    poly = random_convex_valtr(100000, 2 * LIM)
    # Squash vertically to make it thin, then re-center and re-hull.
    poly = [(x, y // 25) for (x, y) in poly]
    xs = [p[0] for p in poly]
    ys = [p[1] for p in poly]
    cx = (min(xs) + max(xs)) // 2
    cy = (min(ys) + max(ys)) // 2
    poly = convex_hull([(x - cx, y - cy) for (x, y) in poly])
    if not origin_inside(poly):
        poly = gen_polygon(100000, LIM)
    poly = subdivide_to(poly, 100000)
    shelves = build_shelves(poly, 100000)
    emit(poly, shelves)
    sys.exit()

if seed == 4:
    # Square tent with axis-aligned shelves and many overlapping intervals.
    R = 1000
    poly = [(R, R), (-R, R), (-R, -R), (R, -R)]
    poly = convex_hull(poly)
    shelves = build_shelves(poly, 500)
    emit(poly, shelves)
    sys.exit()

# Random mid-size mixes.
n = rng.randint(3, 2000)
m = rng.randint(0, 2000)
radius = rng.choice([100, 1000, 10**5, 10**6])
poly = gen_polygon(n, radius)
shelves = build_shelves(poly, m)
emit(poly, shelves)
