"""Tests for p136 (Erasing Edges): rebuild a polygon from edge midpoints.

N <= 10000 midpoints with at most 3 decimal places. Odd N is always
solvable; even N is solvable iff both alternating coordinate sums are
zero. Even YES cases are derived from a real polygon whose vertices sit
on the 0.002 grid (so midpoints have exactly 3 decimals); NO cases
perturb one midpoint by 0.001. Seed 1 is the minimal triangle, seed 2 a
maximal even YES case, seed 3 a maximal even NO case, seed 4 a maximal
odd case with random midpoints.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

NMAX = 10000
R = 10_000_000  # coordinate range in milli-units (+-10000.000)


def fmt(milli):
    return f"{milli / 1000:.3f}"


def emit(midpoints):
    print(len(midpoints))
    for x, y in midpoints:
        print(fmt(x), fmt(y))


def polygon_midpoints(n):
    # Vertices on the even milli grid -> midpoints are integer milli.
    vx = [2 * rng.randint(-R // 2, R // 2) for _ in range(n)]
    vy = [2 * rng.randint(-R // 2, R // 2) for _ in range(n)]
    return [((vx[i] + vx[(i + 1) % n]) // 2,
             (vy[i] + vy[(i + 1) % n]) // 2) for i in range(n)]


def random_midpoints(n, span):
    return [(rng.randint(-span, span), rng.randint(-span, span))
            for _ in range(n)]


if seed == 1:
    emit(polygon_midpoints(3))
elif seed == 2:
    emit(polygon_midpoints(NMAX))
elif seed == 3:
    mids = polygon_midpoints(NMAX)
    i = rng.randrange(NMAX)
    x, y = mids[i]
    mids[i] = (x + rng.choice([-1, 1]), y)
    emit(mids)
elif seed == 4:
    emit(random_midpoints(NMAX - 1, 100_000))
elif seed == 5:
    # Even, x-sums consistent but y-sums off.
    mids = polygon_midpoints(5000)
    i = rng.randrange(5000)
    x, y = mids[i]
    mids[i] = (x, y + 1)
    emit(mids)
else:
    n = rng.randint(3, 5000)
    if n % 2 == 1:
        emit(random_midpoints(n, 1_000_000))
    else:
        mids = polygon_midpoints(n)
        if rng.random() < 0.5:
            i = rng.randrange(n)
            x, y = mids[i]
            mids[i] = (x + rng.choice([-1, 1]), y + rng.choice([-1, 0, 1]))
        emit(mids)
