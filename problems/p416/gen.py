"""Tests for p416 (Optimal Dartboard): single even n, 6 <= n <= 100.

Seed 1 is the minimal n, seeds 2-3 the two largest even values, the
rest cover random even sizes across the range.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(6)
elif seed == 2:
    print(100)
elif seed == 3:
    print(98)
elif seed == 4:
    print(8)
elif seed == 5:
    print(10)
else:
    print(2 * rng.randint(6, 48))
