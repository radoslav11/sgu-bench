"""Tests for p351 (A Mission for a Scout): single circle crater, points
A != B outside it, all input numbers integer; coordinates in [0, 10000],
1 <= RC, D <= 10000. Route = segments + arcs, flight inside crater <= D.

The interesting regimes are: direct segment misses the disc; direct chord
fits in the fuel budget D; tangent + chord-of-length-D + arc + tangent;
and the free chord A-P-Q-B when D is large relative to the wrap angle.

Seed 1 is a tiny direct-flight instance. Seed 2 is maximal: a huge crater
(R=4999) squarely between far-apart A and B with D=1, forcing the long
tangent-arc detour. Seed 3 is maximal the other way: R=10000 centered at
a corner, A and B hugging the circle, large D. Seeds 4-5 are the
statement samples; later seeds mix exact-tangent lines, D equal to the
crossing chord, points one unit off the boundary, and random placements.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LIM = 10000


def emit(ax, ay, bx, by, cx, cy, r, d):
    assert 0 <= min(ax, ay, bx, by, cx, cy) and max(ax, ay, bx, by, cx, cy) <= LIM
    assert 1 <= r <= LIM and 1 <= d <= LIM
    assert (ax, ay) != (bx, by)
    assert (ax - cx) ** 2 + (ay - cy) ** 2 > r * r
    assert (bx - cx) ** 2 + (by - cy) ** 2 > r * r
    print(ax, ay, bx, by)
    print(cx, cy, r)
    print(d)


def rand_outside(cx, cy, r):
    while True:
        x = rng.randint(0, LIM)
        y = rng.randint(0, LIM)
        if (x - cx) ** 2 + (y - cy) ** 2 > r * r:
            return x, y


if seed == 1:
    emit(0, 0, 1, 0, 5000, 5000, 1, 1)
elif seed == 2:
    emit(0, 5000, 10000, 5000, 5000, 5000, 4999, 1)
elif seed == 3:
    emit(10000, 173, 173, 10000, 0, 0, 10000, 10000)
elif seed == 4:
    emit(1, 1, 101, 1, 50, 1, 40, 80)  # statement sample 1
elif seed == 5:
    emit(1, 1, 101, 1, 50, 1, 40, 1)  # statement sample 2
elif seed == 6:
    # Line AB exactly tangent to the circle (distance from center == R).
    r = rng.randint(100, 4000)
    emit(0, 0, 10000, 0, 5000, r, r, rng.randint(1, LIM))
elif seed == 7:
    # AB passes through the center; D exactly equals the chord 2R.
    r = 3000
    emit(0, 5000, 10000, 5000, 5000, 5000, r, 2 * r)
elif seed == 8:
    # Same geometry but D one short of the chord: full-budget flight.
    r = 3000
    emit(0, 5000, 10000, 5000, 5000, 5000, r, 2 * r - 1)
else:
    # Random crater with A and B sampled outside; D spans all regimes.
    while True:
        r = rng.choice([rng.randint(1, 30), rng.randint(30, 2000),
                        rng.randint(2000, 9000)])
        cx = rng.randint(0, LIM)
        cy = rng.randint(0, LIM)
        corners = [(0, 0), (0, LIM), (LIM, 0), (LIM, LIM)]
        if any((x - cx) ** 2 + (y - cy) ** 2 > r * r for x, y in corners):
            break
    if rng.random() < 0.4:
        # Hug the boundary: points just outside the circle.
        def near():
            for _ in range(10000):
                ang = rng.uniform(0, 2 * math.pi)
                rr = r + rng.randint(1, 3) + rng.random()
                x = int(round(cx + rr * math.cos(ang)))
                y = int(round(cy + rr * math.sin(ang)))
                if 0 <= x <= LIM and 0 <= y <= LIM and \
                        (x - cx) ** 2 + (y - cy) ** 2 > r * r:
                    return x, y
            return rand_outside(cx, cy, r)

        ax, ay = near()
        bx, by = near()
    else:
        ax, ay = rand_outside(cx, cy, r)
        bx, by = rand_outside(cx, cy, r)
    while (bx, by) == (ax, ay):
        bx, by = rand_outside(cx, cy, r)
    d = rng.choice([1, rng.randint(1, max(1, r // 2)),
                    rng.randint(max(1, r), min(LIM, 2 * r + 5)), LIM])
    emit(ax, ay, bx, by, cx, cy, r, d)
