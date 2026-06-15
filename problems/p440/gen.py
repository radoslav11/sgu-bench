"""Tests for p440 (Moles and Holes): N holes (3 <= N <= 800), integer
coordinates with |x|,|y| <= 1000, no two holes coincide.

The answer counts ordered "middle" holes P with an unordered pair {Q,R}
whose angle at P is acute and satisfies floor(90/A) = third decimal digit
of cos A. The reference is O(N^2 log N + N^2 K), so cost peaks at the
maximal N with many distinct angles; seeds 2 and 3 use N = 800 with
different point distributions (spread cloud vs. near-collinear bands that
create many small angles). Seed 1 is the minimal N = 3.

The generator only enforces the input constraints (distinct integer points
in range); the validity of the angle condition is left to the reference,
which is judge-accepted.
"""

import random
import sys

MAXN = 800
COORD = 1000


def distinct_points(n, rng, lo=-COORD, hi=COORD):
    pts = set()
    while len(pts) < n:
        pts.add((rng.randint(lo, hi), rng.randint(lo, hi)))
    return list(pts)


def emit(pts):
    print(len(pts))
    for x, y in pts:
        print(x, y)


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit([(0, 0), (10, 0), (0, 10)])
    sys.exit()

if seed == 2:
    # Maximal: 800 holes spread across the whole grid.
    emit(distinct_points(MAXN, rng))
    sys.exit()

if seed == 3:
    # Maximal, structured: points in a few horizontal bands so many triples
    # form small acute angles (stresses the angle-range sweeps).
    pts = set()
    bands = [-800, -400, 0, 400, 800]
    while len(pts) < MAXN:
        pts.add((rng.randint(-COORD, COORD), rng.choice(bands)))
        if len(pts) < MAXN:
            pts.add((rng.randint(-COORD, COORD), rng.randint(-COORD, COORD)))
    emit(list(pts)[:MAXN])
    sys.exit()

if seed == 4:
    # A dense small grid: lots of equal angles and repeated directions.
    side = 20  # 20x20 = 400 distinct lattice points
    pts = [(x, y) for x in range(side) for y in range(side)]
    rng.shuffle(pts)
    emit(pts[: rng.randint(100, 400)])
    sys.exit()

n = rng.randint(3, MAXN)
emit(distinct_points(n, rng))
