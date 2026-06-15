"""Tests for p382 (Cantor Function): a single integer 0 <= n <= 50.

Seed 1 is n=0, seed 2 the maximal n=50, seed 3 n=49 (second largest),
seed 4 n=1, the rest random interior values.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(0)
elif seed == 2:
    print(50)
elif seed == 3:
    print(49)
elif seed == 4:
    print(1)
else:
    print(rng.randint(2, 48))
