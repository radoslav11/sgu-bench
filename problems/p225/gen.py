"""Tests for p225 (Little Knights): 1 <= n <= 10, 0 <= k <= n^2.

Seed 1 is the minimal input. Seeds 2-3 are the heaviest cases (n=10
with k=25 near the count peak and k=50, the largest feasible count).
Other seeds cover k = n^2 boundaries, the special 2x2 board where
knights never attack, and random pairs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 0)
elif seed == 2:
    print(10, 25)
elif seed == 3:
    print(10, 50)
elif seed == 4:
    print(10, 100)
elif seed == 5:
    print(2, 4)
elif seed == 6:
    print(9, 41)
elif seed == 7:
    print(8, 20)
elif seed == 8:
    print(7, 25)
else:
    n = rng.randint(2, 10)
    print(n, rng.randint(0, n * n))
