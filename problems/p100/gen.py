"""Tests for p100 (A+B): 0 < A, B < 10001.

Seeds 1-2 hit the boundary values, the rest are random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1)
elif seed == 2:
    print(10000, 10000)
else:
    print(rng.randint(1, 10000), rng.randint(1, 10000))
