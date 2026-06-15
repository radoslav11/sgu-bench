"""Tests for p469 (Ghostbusters).

Input: W H D (box sizes), N (0..10), then N ghosts X Y Z R with
-1000 < X,Y,Z < 2000 and 0 <= R <= 1000. Every ghost must poke into
the open box, and a positive-radius placement must exist; the
generator verifies the latter by sampling and regenerates if needed.

Seed 1 is the empty unit box, seed 2 ten huge overlapping spheres in
the 1000^3 box, seed 3 ten scattered spheres including radius-0
points, seed 6 spheres centered outside the box.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def pokes_in(w, h, d, g):
    x, y, z, r = g
    qx = min(max(x, 0), w)
    qy = min(max(y, 0), h)
    qz = min(max(z, 0), d)
    dist = math.sqrt((x - qx) ** 2 + (y - qy) ** 2 + (z - qz) ** 2)
    if r == 0:
        return 0 < x < w and 0 < y < h and 0 < z < d
    return dist < r - 1e-9 or (dist == 0 and r > 0)


def radius_at(w, h, d, ghosts, p):
    px, py, pz = p
    best = min(px, w - px, py, h - py, pz, d - pz)
    for x, y, z, r in ghosts:
        dist = math.sqrt((px - x) ** 2 + (py - y) ** 2 + (pz - z) ** 2)
        best = min(best, dist - r)
    return best


def has_positive_spot(w, h, d, ghosts):
    pts = [
        (w * i / 8, h * j / 8, d * k / 8)
        for i in range(1, 8)
        for j in range(1, 8)
        for k in range(1, 8)
    ]
    pts += [
        (rng.uniform(0, w), rng.uniform(0, h), rng.uniform(0, d))
        for _ in range(3000)
    ]
    margin = min(w, h, d) / 100.0
    return any(radius_at(w, h, d, ghosts, p) > margin for p in pts)


def emit(w, h, d, ghosts):
    print(w, h, d)
    print(len(ghosts))
    for g in ghosts:
        print(*g)


def gen_ghosts(w, h, d, n, rad_lo, rad_hi, outside_only=False):
    while True:
        ghosts = []
        while len(ghosts) < n:
            r = rng.randint(rad_lo, rad_hi)
            if outside_only and r > 0:
                axis = rng.randrange(3)
                c = [rng.randint(1, lim - 1) for lim in (w, h, d)]
                lim = (w, h, d)[axis]
                c[axis] = rng.choice(
                    [rng.randint(-min(999, r - 1), -1) if r > 1 else -1,
                     lim + rng.randint(1, max(1, r - 1))]
                )
                g = (c[0], c[1], c[2], r)
            else:
                g = (
                    rng.randint(-999, 1999),
                    rng.randint(-999, 1999),
                    rng.randint(-999, 1999),
                    r,
                )
            if pokes_in(w, h, d, g):
                ghosts.append(g)
        if has_positive_spot(w, h, d, ghosts):
            return ghosts


if seed == 1:
    emit(1, 1, 1, [])
elif seed == 2:
    w = h = d = 1000
    emit(w, h, d, gen_ghosts(w, h, d, 10, 200, 1000))
elif seed == 3:
    w = h = d = 1000
    small = gen_ghosts(w, h, d, 7, 30, 250)
    points = gen_ghosts(w, h, d, 3, 0, 0)
    emit(w, h, d, small + points)
elif seed == 4:
    emit(1000, 1, 17, [])
elif seed == 5:
    w, h, d = 500, 700, 900
    emit(w, h, d, gen_ghosts(w, h, d, 10, 0, 0))
elif seed == 6:
    w = h = d = 800
    emit(w, h, d, gen_ghosts(w, h, d, 10, 100, 1000, outside_only=True))
else:
    w = rng.randint(2, 1000)
    h = rng.randint(2, 1000)
    d = rng.randint(2, 1000)
    n = rng.randint(0, 10)
    emit(w, h, d, gen_ghosts(w, h, d, n, 0, rng.choice([50, 300, 1000])))
