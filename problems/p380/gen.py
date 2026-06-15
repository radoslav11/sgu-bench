"""Tests for p380 (Synchronised Alpinism): polyline heights y1..yn with
y1 = yn = 0 (the printed constraints are garbled in the statement, so we
stay at a moderate n <= 100000 and |y| <= 10^6).

Seed 1 minimal flat, seed 2 maximal random signed heights, seed 3
maximal non-negative sawtooth (classic always-YES shape), later seeds
use tiny height alphabets so both YES and NO verdicts appear, plus deep
valleys near the ends.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXN = 100000
MAXY = 10**6


def emit(h):
    h[0] = 0
    h[-1] = 0
    print(len(h))
    print(" ".join(map(str, h)))


if seed == 1:
    emit([0, 0])
elif seed == 2:
    emit([rng.randint(-MAXY, MAXY) for _ in range(MAXN)])
elif seed == 3:
    emit([rng.randint(0, MAXY) for _ in range(MAXN)])
elif seed == 4:
    # deep valleys right next to both ends, then a tall ridge
    n = MAXN
    h = [rng.randint(-5, 5) for _ in range(n)]
    h[1] = -MAXY
    h[n - 2] = -MAXY
    h[n // 2] = MAXY
    emit(h)
elif seed == 5:
    emit([0] * rng.randint(2, 1000))
else:
    n = rng.randint(2, 60)
    h = [rng.randint(-2, 2) for _ in range(n)]
    emit(h)
