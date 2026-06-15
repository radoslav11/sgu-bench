"""Tests for p217 (Two Cylinders): volume of two perpendicular
intersecting cylinders, radii 1 <= R1, R2 <= 100.

The input is just two real numbers, so every test is tiny. Seed 1 is
the minimal pair (1, 1), seed 2 the maximal equal pair (100, 100) and
seed 3 the most skewed pair (1, 100). Later seeds mix integer radii,
radii with many decimals and nearly-equal radii.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print("1 1")
elif seed == 2:
    print("100 100")
elif seed == 3:
    print("1 100")
elif seed == 4:
    print("100 1")
elif seed == 5:
    r = round(rng.uniform(1, 100), 6)
    print(f"{r:.6f} {r + 0.000001:.6f}")
elif seed == 6:
    print(rng.randint(1, 100), rng.randint(1, 100))
else:
    print(f"{rng.uniform(1, 100):.6f} {rng.uniform(1, 100):.6f}")
