"""Tests for p147 (Black-white king): N <= 1e6, three distinct king
positions on an NxN board.

Seed 1 is the smallest board (N=2). Seeds 2-3 are maximal N=1e6: kings
in opposite corners with the invisible king on/off their meeting path.
Later seeds randomize positions on big boards plus adversarial layouts:
kings already adjacent, the invisible king exactly midway, collinear
rows/columns, and near-board-edge squeezes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def distinct_positions(rng, n, k):
    pts = set()
    while len(pts) < k:
        pts.add((rng.randint(1, n), rng.randint(1, n)))
    return list(pts)


if seed == 1:
    n = 2
    p1, p2, p3 = (1, 1), (2, 2), (1, 2)
elif seed == 2:
    n = 10**6
    p1, p2, p3 = (1, 1), (n, n), (n // 2, n // 2)  # invisible king midway
elif seed == 3:
    n = 10**6
    p1, p2, p3 = (1, n), (n, 1), (1, 1)  # invisible king off the path
elif seed == 4:
    n = 10**6
    p1, p2, p3 = (5, 5), (6, 6), (n, n)  # kings already adjacent
elif seed == 5:
    n = 10**6
    p1, p2, p3 = (1, 500000), (n, 500000), (500000, 500000)  # same row
else:
    n = rng.choice([rng.randint(2, 10), rng.randint(10, 1000), 10**6])
    if seed % 3 == 0 and n >= 4:
        # collinear configuration with the invisible king between them
        row = rng.randint(1, n)
        c1, c2 = sorted(rng.sample(range(1, n + 1), 2))
        cm = rng.randint(c1, c2)
        pts = {(c1, row), (c2, row), (cm, row)}
        if len(pts) < 3:
            pts = set(distinct_positions(rng, n, 3))
        p1, p2, p3 = list(pts)
    else:
        p1, p2, p3 = distinct_positions(rng, n, 3)

print(n)
print(p1[0], p1[1])
print(p2[0], p2[1])
print(p3[0], p3[1])
