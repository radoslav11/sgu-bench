"""Tests for p223 (Little Kings): 1 <= n <= 10, 0 <= k <= n^2.

Seed 1 is the minimal input, seed 2 maximizes the DP state space
(n=10, k=100), seed 3 is another max board with k near the feasible
peak, the rest sample boundary and random (n, k) pairs.
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
    print(10, 25)
elif seed == 4:
    print(10, 13)
elif seed == 5:
    print(1, 1)
elif seed == 6:
    print(9, 81)
else:
    n = rng.randint(2, 10)
    print(n, rng.randint(0, n * n))
