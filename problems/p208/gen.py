"""Tests for p208 (Toral Tickets): a single pair N M with 1 <= N, M <= 20.

Seed 1 is the minimal 1 1, seed 2 the maximal square 20 20 (the square
case doubles the symmetry group), seed 3 the maximal non-square 19 20.
Later seeds fix interesting pairs (1 x M, primes, sample) and random ones.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

fixed = {
    1: (1, 1),
    2: (20, 20),
    3: (19, 20),
    4: (2, 2),
    5: (2, 3),
    6: (1, 20),
    7: (17, 17),
}

if seed in fixed:
    n, m = fixed[seed]
else:
    n, m = rng.randint(1, 20), rng.randint(1, 20)

print(n, m)
