"""Tests for p521 (North-East): n<=1e5 distinct points, coords in
[-1e6, 1e6]; longest strictly-NE increasing chain, lists A (some max
chain) and B (every max chain).

Seed 1 is a single point. Seeds 2 and 3 are maximal n=1e5: seed 2 is a
strict diagonal (one unique longest chain through all points), seed 3 is
a wide random cloud with many ties on x and y (lots of equal coordinates
forcing batch handling and many alternative chains). Later seeds add
anti-chains, grids, and small random clouds.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)
LO, HI = -10 ** 6, 10 ** 6


def emit(pts):
    print(len(pts))
    out = []
    for x, y in pts:
        out.append(f"{x} {y}")
    sys.stdout.write("\n".join(out) + "\n")


def distinct_random(n):
    seen = set()
    pts = []
    while len(pts) < n:
        x = rng.randint(LO, HI)
        y = rng.randint(LO, HI)
        if (x, y) in seen:
            continue
        seen.add((x, y))
        pts.append((x, y))
    return pts


if seed == 1:
    emit([(0, 0)])
elif seed == 2:
    n = 10 ** 5
    # strict diagonal: single unique longest chain of length n
    base = LO
    pts = [(base + i * 20, base + i * 20) for i in range(n)]
    rng.shuffle(pts)
    emit(pts)
elif seed == 3:
    n = 10 ** 5
    # narrow coordinate window forces many equal x and y values
    seen = set()
    pts = []
    span = 300
    while len(pts) < n:
        x = rng.randint(-span, span)
        y = rng.randint(-span, span)
        if (x, y) in seen:
            continue
        seen.add((x, y))
        pts.append((x, y))
    emit(pts)
elif seed == 4:
    # large anti-chain: x increasing, y decreasing -> longest chain = 1
    n = 10 ** 5
    pts = [(i, n - i) for i in range(n)]
    rng.shuffle(pts)
    emit(pts)
elif seed == 5:
    # grid: many equal coordinates, many longest chains, large B unlikely
    side = 300
    pts = [(i, j) for i in range(side) for j in range(side)]
    rng.shuffle(pts)
    emit(pts[: 10 ** 5])
else:
    n = rng.randint(2, 2000)
    pts = distinct_random(n)
    emit(pts)
