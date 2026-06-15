"""Tests for p522 (Oil Wells): n<=400 distinct wells, start and wells in
[-400, 400]; minimal two-phase rectilinear fence area plus a route.

Seed 1 is a single well next to the start. Seeds 2 and 3 are maximal
n=400: seed 2 spreads wells across the whole [-400,400] square so all
four quadrant orientations must be tried, seed 3 packs wells into one
quadrant of the start with many shared rows/columns (dense staircase).
Later seeds probe collinear wells, wells on the start, and a case where
some wells straddle the start so most orientations are infeasible.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)
LO, HI = -400, 400


def emit(x0, y0, wells):
    print(len(wells), x0, y0)
    out = [f"{wx} {wy}" for wx, wy in wells]
    sys.stdout.write("\n".join(out) + "\n")


def distinct(n, xlo, xhi, ylo, yhi):
    seen = set()
    pts = []
    cap = (xhi - xlo + 1) * (yhi - ylo + 1)
    n = min(n, cap)
    while len(pts) < n:
        p = (rng.randint(xlo, xhi), rng.randint(ylo, yhi))
        if p in seen:
            continue
        seen.add(p)
        pts.append(p)
    return pts


if seed == 1:
    emit(0, 0, [(1, 1)])
elif seed == 2:
    x0, y0 = 0, 0
    wells = distinct(400, LO, HI, LO, HI)
    emit(x0, y0, wells)
elif seed == 3:
    # dense cluster in the NE quadrant of the start, shared rows/cols
    x0, y0 = -400, -400
    wells = distinct(400, -400, -360, -400, -360)
    emit(x0, y0, wells)
elif seed == 4:
    # all wells collinear horizontally to one side of the start
    x0, y0 = 0, 0
    xs = rng.sample(range(1, 401), 400)
    wells = [(x, 50) for x in xs]
    emit(x0, y0, wells)
elif seed == 5:
    # wells straddle the start: many orientations infeasible, but the
    # ones aligned with the spread still work
    x0, y0 = 0, 0
    wells = distinct(400, -200, 200, -200, 200)
    emit(x0, y0, wells)
elif seed == 6:
    # start coincides with a well, others in NE quadrant
    x0, y0 = 5, 5
    wells = [(5, 5)] + distinct(399, 6, 400, 6, 400)
    emit(x0, y0, wells)
else:
    x0 = rng.randint(LO, HI)
    y0 = rng.randint(LO, HI)
    n = rng.randint(1, 400)
    # bias to a quadrant so a solution usually exists
    sx = rng.choice([1, -1])
    sy = rng.choice([1, -1])
    xlo, xhi = (x0, min(HI, x0 + 200)) if sx == 1 else (max(LO, x0 - 200), x0)
    ylo, yhi = (y0, min(HI, y0 + 200)) if sy == 1 else (max(LO, y0 - 200), y0)
    wells = distinct(n, xlo, xhi, ylo, yhi)
    emit(x0, y0, wells)
