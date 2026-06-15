"""Tests for p481 (Hero of Our Time): single integer n, 3 <= n <= 5000.

Seed 1 is minimal (n = 3), seeds 2-3 maximal (n = 5000 and 4999 — the
answer has ~18500 digits), seeds 4-5 the small boundary values, the
rest random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(3)
elif seed == 2:
    print(5000)
elif seed == 3:
    print(4999)
elif seed == 4:
    print(4)
elif seed == 5:
    print(5)
else:
    print(rng.randint(3, 5000))
