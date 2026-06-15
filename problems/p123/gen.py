"""Tests for p123 (sum of first K Fibonacci numbers): 0 < K < 41.

Seeds 1-2 are the boundaries, the rest random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1)
elif seed == 2:
    print(40)
else:
    print(rng.randint(2, 39))
