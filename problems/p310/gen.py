"""Tests for p310 (Hippopotamus): three integers n, m, k.

1 <= n <= 60, 1 <= m <= 15, 0 <= k <= m <= n. Seed 1 is minimal, seed 2
is the maximal bitmask case (n=60, m=15, mid k), seed 3 is n=60 m=15 k=0
(answer 2^60, the largest output). Seeds 4-5 pin k at the extremes,
the rest are random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1, 0)
elif seed == 2:
    print(60, 15, 7)
elif seed == 3:
    print(60, 15, 0)
elif seed == 4:
    print(60, 15, 15)
elif seed == 5:
    print(60, 15, 1)
else:
    n = rng.randint(1, 60)
    m = rng.randint(1, min(15, n))
    k = rng.randint(0, m)
    print(n, m, k)
