"""Tests for p403 (Scientific Problem): single integer 1 <= x <= 10^9.

Seed 1 is the minimum, seed 2 the maximum, seed 3 one below the
maximum, the rest are random over the whole range.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1)
elif seed == 2:
    print(10**9)
elif seed == 3:
    print(10**9 - 1)
else:
    print(rng.randint(1, 10**9))
