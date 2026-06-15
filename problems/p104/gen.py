"""Tests for p104 (Little shop of flowers): 1 <= F <= V <= 100,
-50 <= Aij <= 50.

Seed 1 is the 1x1 minimum, seed 2 a 100x100 random max, seed 3 a
100x100 max with all values equal (every arrangement optimal, heavy
ties), seed 4 all values -50 (forced negative optimum), seed 5 has
F = V (no freedom), the rest are random mid sizes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(f, v, value):
    print(f, v)
    for _ in range(f):
        print(" ".join(str(value()) for _ in range(v)))


if seed == 1:
    emit(1, 1, lambda: rng.randint(-50, 50))
elif seed == 2:
    emit(100, 100, lambda: rng.randint(-50, 50))
elif seed == 3:
    emit(100, 100, lambda: 7)
elif seed == 4:
    emit(60, 100, lambda: -50)
elif seed == 5:
    n = rng.randint(2, 100)
    emit(n, n, lambda: rng.randint(-50, 50))
else:
    f = rng.randint(1, 100)
    v = rng.randint(f, 100)
    emit(f, v, lambda: rng.randint(-50, 50))
