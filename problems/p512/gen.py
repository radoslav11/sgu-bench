"""Tests for p512 (Friendly Points): n <= 100000 distinct points, |coord| <= 1e9.

Seed 1 is the minimal case (n = 1). Seed 2 is maximal random: n = 100000
distinct points spread over the full coordinate range. Seed 3 is a second
max-size test shaped as a near-square grid, which maximizes the number of
friend pairs and stresses the divide-and-conquer merge. Later seeds cover
adversarial structure: all points on a vertical line (single x group), all on
a horizontal line, a diagonal, heavy duplicate-x columns, and random
mid-size cases.
"""

import random
import sys

NMAX = 100000
C = 10**9

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(pts):
    out = [str(len(pts))]
    out += [f"{x} {y}" for x, y in pts]
    sys.stdout.write("\n".join(out) + "\n")


def distinct_random(n, lo, hi, rng):
    seen = set()
    pts = []
    while len(pts) < n:
        x = rng.randint(lo, hi)
        y = rng.randint(lo, hi)
        if (x, y) not in seen:
            seen.add((x, y))
            pts.append((x, y))
    return pts


if seed == 1:
    emit([(rng.randint(-C, C), rng.randint(-C, C))])
    sys.exit()

if seed == 2:
    # maximal random, full coordinate span
    pts = distinct_random(NMAX, -C, C, rng)
    emit(pts)
    sys.exit()

if seed == 3:
    # maximal near-square grid: 317*316 ~= 100172, take first 100000
    side = 317
    pts = []
    for gx in range(side):
        for gy in range(side):
            pts.append((gx * 3, gy * 3))
            if len(pts) == NMAX:
                break
        if len(pts) == NMAX:
            break
    # shift into the negative range too and shuffle order
    pts = [(x - 400, y - 400) for x, y in pts]
    rng.shuffle(pts)
    emit(pts)
    sys.exit()

if seed == 4:
    # all on a vertical line: single x group, distinct y
    x0 = rng.randint(-C, C)
    ys = rng.sample(range(-C, C + 1), NMAX)
    pts = [(x0, y) for y in ys]
    emit(pts)
    sys.exit()

if seed == 5:
    # all on a horizontal line: distinct x, same y
    y0 = rng.randint(-C, C)
    xs = rng.sample(range(-C, C + 1), NMAX)
    pts = [(x, y0) for x in xs]
    emit(pts)
    sys.exit()

if seed == 6:
    # diagonal line: strictly increasing x and y
    base = rng.sample(range(-C, C + 1), NMAX)
    base.sort()
    pts = [(v, v) for v in base]
    rng.shuffle(pts)
    emit(pts)
    sys.exit()

if seed == 7:
    # few x columns, many points per column (heavy duplicate-x)
    cols = rng.sample(range(-C, C + 1), 50)
    pts = []
    per = NMAX // len(cols)
    used = set()
    for cx in cols:
        ys = rng.sample(range(-C, C + 1), per)
        for y in ys:
            pts.append((cx, y))
    emit(pts)
    sys.exit()

# seeds 8+: random mid-size, smaller coordinate window for clustering
n = rng.randint(2, 5000)
span = rng.choice([50, 1000, 10**6, C])
pts = distinct_random(n, -span, span, rng)
emit(pts)
