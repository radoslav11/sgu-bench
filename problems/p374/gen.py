"""Tests for p374 (Save Vasya): 1 <= a, b <= 100, 1 <= k <= 20.

Seed 1 is minimal, seeds 2-3 maximal (biggest answer values), the rest
random with small/large mixes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1, 1)
elif seed == 2:
    print(100, 100, 20)
elif seed == 3:
    print(99, 100, 20)
elif seed == 4:
    print(1, 1, 20)
else:
    print(rng.randint(1, 100), rng.randint(1, 100), rng.randint(1, 20))
