"""Tests for p419 (Hexagonal Walkaround): x y d2 d1 b on one line.

-1e12 <= x, y <= 1e12, 0 <= d1, d2 <= 5, 2 <= b <= 10. Seed 1 is the
trivial stay-in-place case, seeds 2-3 hit the coordinate extremes, the
rest mix small/huge coordinates, b boundaries and all direction pairs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LIM = 10 ** 12

if seed == 1:
    print(0, 0, 0, 0, 2)
elif seed == 2:
    print(LIM, LIM, 3, 0, 2)
elif seed == 3:
    print(-LIM, -LIM, 0, 5, 10)
elif seed == 4:
    print(LIM, -LIM, 2, 2, 2)
elif seed == 5:
    print(-LIM, LIM, 5, 1, 10)
elif seed == 6:
    # small neighbourhood of the origin, forcing wraparound laps
    print(rng.randint(-3, 3), rng.randint(-3, 3), rng.randint(0, 5),
          rng.randint(0, 5), rng.randint(2, 10))
elif seed == 7:
    # on-axis target far away
    print(rng.choice([-LIM, LIM, 0]), 0, rng.randint(0, 5),
          rng.randint(0, 5), rng.randint(2, 10))
else:
    print(rng.randint(-LIM, LIM), rng.randint(-LIM, LIM),
          rng.randint(0, 5), rng.randint(0, 5), rng.randint(2, 10))
