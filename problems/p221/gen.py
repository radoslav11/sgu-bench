"""Tests for p221 (Big Bishops): 1 <= n <= 50, 0 <= k <= n^2.

Seed 1 is minimal (n=1, k=0), seed 2 is n=50 with k=50 (huge bignum
answer), seed 3 is n=50 with k=98 (max placeable count), the rest cover
k=0, just-impossible k, the n^2 cap and random pairs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 0)
elif seed == 2:
    print(50, 50)
elif seed == 3:
    print(50, 98)
elif seed == 4:
    print(50, 99)
elif seed == 5:
    print(50, 2500)
elif seed == 6:
    print(50, 0)
elif seed == 7:
    print(1, 1)
elif seed == 8:
    n = rng.randint(2, 49)
    print(n, 2 * n - 2)
else:
    n = rng.randint(1, 50)
    print(n, rng.randint(0, n * n))
