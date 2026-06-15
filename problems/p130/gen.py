"""Tests for p130 (Circle): single integer k, 1 <= k <= 30.

Seed 1 is the minimum k=1, seeds 2-3 the largest values (k=30, k=29),
the rest cover the remaining range randomly.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1)
elif seed == 2:
    print(30)
elif seed == 3:
    print(29)
elif seed == 4:
    print(2)
else:
    print(rng.randint(3, 28))
