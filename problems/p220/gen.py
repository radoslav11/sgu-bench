"""Tests for p220 (Little Bishops): 1 <= n <= 10, 0 <= k <= n^2.

Seed 1 is minimal (n=1, k=0), seed 2 maximal n with k at the n^2 cap,
seed 3 maximal n with k = 2n-2 (largest placeable count), the rest cover
k = 0, k just above 2n-2, and random pairs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 0)
elif seed == 2:
    print(10, 100)
elif seed == 3:
    print(10, 18)
elif seed == 4:
    print(10, 0)
elif seed == 5:
    print(10, 19)
elif seed == 6:
    print(1, 1)
elif seed == 7:
    n = rng.randint(2, 9)
    print(n, 2 * n - 2)
else:
    n = rng.randint(1, 10)
    print(n, rng.randint(0, n * n))
