"""Tests for p496 (L-Shapes): count perpendicular segment pairs that share
an endpoint.

Input: N (1 <= N <= 5000) then N lines "x1 y1 x2 y2" with coordinates in
[-10000, 10000] and the two endpoints distinct. Seed 2 is a star where
all segments share one hub point with many perpendicular orientations,
the heaviest case for the grouping. Seed 3 packs segments densely on a
small grid so many endpoints coincide.
"""

import random
import sys

N_MAX = 5000
COORD = 10000


def emit(segs):
    out = [str(len(segs))]
    for x1, y1, x2, y2 in segs:
        out.append(f"{x1} {y1} {x2} {y2}")
    sys.stdout.write("\n".join(out) + "\n")


def perp(dx, dy):
    return (-dy, dx)


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    # Minimal: a single segment, no pairs possible.
    emit([(0, 0, 0, 1)])

elif seed == 2:
    # Star: every segment touches the origin; pick many integer directions
    # so that perpendicular partners abound, maximising the inner-map work
    # at one shared point.
    dirs = []
    for dx in range(-40, 41):
        for dy in range(-40, 41):
            if (dx, dy) != (0, 0):
                dirs.append((dx, dy))
    segs = []
    for _ in range(N_MAX):
        dx, dy = rng.choice(dirs)
        # scale so the far endpoint stays in range
        s = max(abs(dx), abs(dy))
        k = rng.randint(1, COORD // s)
        segs.append((0, 0, dx * k, dy * k))
    emit(segs)

elif seed == 3:
    # Dense small grid: many axis-aligned unit segments so endpoints
    # coincide heavily and L-shapes are plentiful.
    g = 70  # ~70*70 region
    segs = []
    while len(segs) < N_MAX:
        x = rng.randint(-g, g)
        y = rng.randint(-g, g)
        if rng.random() < 0.5:
            segs.append((x, y, x, y + 1))
        else:
            segs.append((x, y, x + 1, y))
    emit(segs)

elif seed == 4:
    # Random axis-aligned segments over the full coordinate range.
    segs = []
    while len(segs) < N_MAX:
        x1 = rng.randint(-COORD, COORD)
        y1 = rng.randint(-COORD, COORD)
        if rng.random() < 0.5:
            y2 = rng.randint(-COORD, COORD)
            if y2 != y1:
                segs.append((x1, y1, x1, y2))
        else:
            x2 = rng.randint(-COORD, COORD)
            if x2 != x1:
                segs.append((x1, y1, x2, y1))
    emit(segs)

elif seed == 5:
    # Random general-direction segments; few coincidences, mostly zero
    # L-shapes, exercises the gcd-normalisation path.
    segs = []
    while len(segs) < N_MAX:
        x1 = rng.randint(-COORD, COORD)
        y1 = rng.randint(-COORD, COORD)
        x2 = rng.randint(-COORD, COORD)
        y2 = rng.randint(-COORD, COORD)
        if (x1, y1) != (x2, y2):
            segs.append((x1, y1, x2, y2))
    emit(segs)

elif seed == 6:
    # Many identical and near-identical segments sharing two hubs, plus
    # their perpendicular partners (distinct pairs even when coincident).
    segs = []
    pts = [(rng.randint(-20, 20), rng.randint(-20, 20)) for _ in range(8)]
    while len(segs) < N_MAX:
        px, py = rng.choice(pts)
        dx, dy = rng.choice([(1, 0), (0, 1), (1, 1), (1, -1), (2, 1), (1, 2)])
        if rng.random() < 0.5:
            dx, dy = perp(dx, dy)
        k = rng.randint(1, 200)
        nx, ny = px + dx * k, py + dy * k
        if -COORD <= nx <= COORD and -COORD <= ny <= COORD:
            segs.append((px, py, nx, ny))
    emit(segs)

else:
    # General randomised mid-size mix.
    n = rng.randint(1, N_MAX)
    segs = []
    while len(segs) < n:
        x1 = rng.randint(-COORD, COORD)
        y1 = rng.randint(-COORD, COORD)
        x2 = rng.randint(-COORD, COORD)
        y2 = rng.randint(-COORD, COORD)
        if (x1, y1) != (x2, y2):
            segs.append((x1, y1, x2, y2))
    emit(segs)
