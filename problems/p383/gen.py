"""Tests for p383 (Caravans): n oases at distinct integer points (not all
collinear), q queries; answer is the bottleneck (max edge) on the path
between the two oases in the Euclidean minimum spanning tree.

The reference builds the EMST from Delaunay/Voronoi edges (O(n log n)) and
answers queries with parallel binary search, so cost peaks at the maximal
n and q. Seeds 2 and 3 use n = q = 100000 with different point
distributions (uniform cloud vs. clustered, which stresses the Delaunay
and produces long MST paths). Seed 1 is the minimal non-degenerate case
(3 non-collinear points). Later seeds randomize sizes and include repeated
endpoints (answer 0) and grid points.

Constraints used: 3 <= n <= 100000, |x|,|y| <= 10000, 1 <= q <= 100000
(consistent with the editorial's "~10^5 points" and the 4.25s / 256 MB
limits). The coordinate magnitude is deliberately capped at 10000: the
reference's Fortune's-algorithm Voronoi is numerically fragile and
segfaults on some wide-spread (|coord| ~ 10^6) point sets, while it is
reliably stable for |coord| <= 10000 even at n = 100000. Inputs only need
to be valid; expected outputs come from the judge-accepted reference.
"""

import random
import sys

MAXN = 100000
MAXQ = 100000
COORD = 10000


def distinct_points(n, rng, lo, hi):
    pts = set()
    while len(pts) < n:
        pts.add((rng.randint(lo, hi), rng.randint(lo, hi)))
    return list(pts)


def ensure_non_collinear(pts):
    # Random 2D points are essentially never all collinear, but guarantee it.
    (x0, y0), (x1, y1) = pts[0], pts[1]
    for (x, y) in pts[2:]:
        if (x1 - x0) * (y - y0) - (y1 - y0) * (x - x0) != 0:
            return pts
    pts[-1] = (pts[-1][0], pts[-1][1] + 1)
    return pts


def emit(pts, queries):
    print(len(pts))
    out = [f"{x} {y}" for x, y in pts]
    print("\n".join(out))
    print(len(queries))
    print("\n".join(f"{s} {t}" for s, t in queries))


def random_queries(n, q, rng):
    return [(rng.randint(1, n), rng.randint(1, n)) for _ in range(q)]


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit([(0, 0), (50, 10), (150, 0)], [(1, 2), (1, 3), (2, 3)])
    sys.exit()

if seed == 2:
    # Maximal uniform cloud.
    pts = ensure_non_collinear(distinct_points(MAXN, rng, -COORD, COORD))
    emit(pts, random_queries(len(pts), MAXQ, rng))
    sys.exit()

if seed == 3:
    # Maximal clustered: several tight blobs so the MST has long bridges
    # between clusters (large bottleneck edges).
    pts = set()
    centers = [
        (rng.randint(-COORD, COORD), rng.randint(-COORD, COORD))
        for _ in range(20)
    ]
    while len(pts) < MAXN:
        cx, cy = rng.choice(centers)
        x = min(COORD, max(-COORD, cx + rng.randint(-2000, 2000)))
        y = min(COORD, max(-COORD, cy + rng.randint(-2000, 2000)))
        pts.add((x, y))
    pts = ensure_non_collinear(list(pts))
    emit(pts, random_queries(len(pts), MAXQ, rng))
    sys.exit()

if seed == 4:
    # Dense lattice block: many equal-length edges.
    side = 300  # up to 90000 points
    pts = [(x, y) for x in range(side) for y in range(side)]
    rng.shuffle(pts)
    pts = pts[: rng.randint(40000, side * side)]
    queries = random_queries(len(pts), MAXQ, rng)
    # include some identical-endpoint queries (answer 0)
    for _ in range(100):
        v = rng.randint(1, len(pts))
        queries.append((v, v))
    emit(pts, queries)
    sys.exit()

n = rng.randint(3, 5000)
pts = ensure_non_collinear(distinct_points(n, rng, -COORD, COORD))
q = rng.randint(1, 5000)
queries = random_queries(len(pts), q, rng)
for _ in range(min(50, q)):
    v = rng.randint(1, len(pts))
    queries.append((v, v))
emit(pts, queries)
