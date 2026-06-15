"""Tests for p417 (Heavy Disc): integers x0, y0, r.

-100 <= x0, y0 <= 100, 1 <= r <= 100, origin strictly outside the disc
(x0^2 + y0^2 > r^2). Seed 1 is a tiny disc, seed 2 the largest radius
with the disc boundary nearly touching the origin, seed 3 the farthest
large disc; remaining seeds are random valid triples, including cases
with the boundary passing within distance 1 of the origin.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1, 1)
elif seed == 2:
    print(100, 1, 100)
elif seed == 3:
    print(-100, -100, 100)
elif seed == 4:
    print(0, 2, 1)
elif seed == 5:
    print(-100, 0, 99)
else:
    while True:
        x0 = rng.randint(-100, 100)
        y0 = rng.randint(-100, 100)
        s2 = x0 * x0 + y0 * y0
        if s2 <= 1:
            continue
        rmax = min(100, int((s2 - 1) ** 0.5) + 1)
        while rmax * rmax >= s2:
            rmax -= 1
        if rmax < 1:
            continue
        r = rng.randint(max(1, rmax - 2), rmax) if seed % 2 else rng.randint(1, rmax)
        print(x0, y0, r)
        break
