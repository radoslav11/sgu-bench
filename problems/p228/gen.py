"""Tests for p228 (Archipelago): regular N-gon, 3 <= N <= 150, vertices
numbered clockwise; given coordinates of islands N1 and N2 (|c| <= 1e6).

Seed 1 is the minimal N=3, seed 2 the maximal N=150 with adjacent given
vertices placed ~2.8e6 apart (largest circumradius), seed 3 N=150 with
near-opposite vertices at coordinate extremes, seed 4 an exactly
diametric pair, the rest random (including N1 > N2).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def coord():
    return rng.uniform(-1e6, 1e6)


def emit(n, n1, n2, x1, y1, x2, y2):
    print(n, n1, n2)
    print(f"{x1:.4f} {y1:.4f}")
    print(f"{x2:.4f} {y2:.4f}")


if seed == 1:
    emit(3, 1, 2, 0.0, 0.0, 10.0, 0.0)
elif seed == 2:
    # Adjacent vertices of a 150-gon at opposite corners of the plane:
    # circumradius ~ 6.7e7, the numerically heaviest case.
    emit(150, 1, 2, -999999.1234, -999998.5678, 999997.4321, 999996.8765)
elif seed == 3:
    emit(150, 17, 92, -1000000.0, 1000000.0, 1000000.0, -1000000.0)
elif seed == 4:
    # Exactly diametric pair (N even, |N1 - N2| = N/2).
    emit(100, 13, 63, coord(), coord(), coord(), coord())
else:
    n = rng.randint(3, 150)
    n1, n2 = rng.sample(range(1, n + 1), 2)
    while True:
        x1, y1, x2, y2 = coord(), coord(), coord(), coord()
        if abs(x1 - x2) + abs(y1 - y2) > 1.0:
            break
    emit(n, n1, n2, x1, y1, x2, y2)
