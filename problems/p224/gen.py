"""Tests for p224 (Little Queens): 1 <= n <= 10, 0 <= k <= n^2.

Seed 1 is the minimal input. Seeds 2-3 are the heaviest searches
(n=10 with k=6 and k=7, where the number of partial placements
peaks). Remaining seeds cover boundaries and random pairs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 0)
elif seed == 2:
    print(10, 6)
elif seed == 3:
    print(10, 7)
elif seed == 4:
    print(10, 5)
elif seed == 5:
    print(10, 10)
elif seed == 6:
    print(10, 100)
elif seed == 7:
    print(9, 6)
else:
    n = rng.randint(2, 10)
    print(n, rng.randint(0, n * n))
