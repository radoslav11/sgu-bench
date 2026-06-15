"""Tests for p222 (Little Rooks): 1 <= n <= 10, 0 <= k <= n^2.

Seed 1 is minimal (n=1, k=0), seed 2 maximal (n=10, k=100), seed 3 the
largest feasible placement (n=10, k=10), the rest cover k=0, k=n+1 and
random pairs.
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
    print(10, 10)
elif seed == 4:
    print(10, 0)
elif seed == 5:
    print(10, 11)
elif seed == 6:
    print(1, 1)
elif seed == 7:
    print(10, 7)
else:
    n = rng.randint(1, 10)
    print(n, rng.randint(0, n * n))
