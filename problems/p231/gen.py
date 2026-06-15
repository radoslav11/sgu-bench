"""Tests for p231 (Prime Sum): pairs of primes (A, B) with A+B prime <= N.

The input is a single integer N <= 10^6. Seed 1 is the minimal N=1,
seeds 2-3 are maximal (10^6 and 10^6 - 1), the rest cover tiny edge
values (the first N where pairs appear) and random N.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1)
elif seed == 2:
    print(1000000)
elif seed == 3:
    print(999999)
elif seed == 4:
    print(5)  # first N with a pair: 2 + 3 = 5
elif seed == 5:
    print(rng.choice([2, 3, 4, 6, 7]))
else:
    print(rng.randint(10, 1000000))
