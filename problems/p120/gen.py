"""Tests for p120 (Archipelago): 3 <= N <= 150, islands of a regular
N-gon numbered clockwise, two island coordinates given with 4 decimals,
each in (-2e6, 2e6).

A random regular polygon is generated (center, radius, rotation), then
two of its vertices are printed rounded to 4 decimals. Seed 1 is the
sample-sized N = 4, seed 2 the maximal N = 150 with adjacent given
islands on a huge polygon (worst error amplification), seed 3 N = 150
with nearly opposite islands, seed 4 diametrically opposite islands on
an even N, seed 5 a tiny-radius polygon, the rest random.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, n1, n2, cx, cy, r, phase):
    def vertex(k):
        ang = phase - 2 * math.pi * (k - 1) / n
        return cx + r * math.cos(ang), cy + r * math.sin(ang)

    x1, y1 = vertex(n1)
    x2, y2 = vertex(n2)
    assert all(abs(v) < 2e6 for v in (x1, y1, x2, y2))
    print(n, n1, n2)
    print(f"{x1:.4f} {y1:.4f}")
    print(f"{x2:.4f} {y2:.4f}")


phase = rng.uniform(0, 2 * math.pi)

if seed == 1:
    emit(4, 1, 3, 1.0, 1.0, math.sqrt(2), phase)
elif seed == 2:
    emit(150, 70, 71, 0.0, 0.0, 1.9e6, phase)
elif seed == 3:
    emit(150, 1, 76, rng.uniform(-1e5, 1e5), rng.uniform(-1e5, 1e5),
         1.5e6, phase)
elif seed == 4:
    emit(100, 25, 75, rng.uniform(-1e3, 1e3), rng.uniform(-1e3, 1e3),
         rng.uniform(10, 1000), phase)
elif seed == 5:
    emit(17, 5, 9, rng.uniform(-100, 100), rng.uniform(-100, 100),
         0.05, phase)
else:
    n = rng.randint(3, 150)
    n1, n2 = rng.sample(range(1, n + 1), 2)
    r = rng.uniform(1, 1e5)
    cx, cy = rng.uniform(-1e6, 1e6), rng.uniform(-1e6, 1e6)
    emit(n, n1, n2, cx, cy, r, phase)
