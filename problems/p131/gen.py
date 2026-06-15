"""Tests for p131 (Hardwood floor): tilings of an M x N hall, 1 <= M, N <= 9.

Seed 1 is the 1x1 board (answer 0), seeds 2-3 the largest boards 9x9 and
8x9, seed 4 the sample, the rest random dimensions including odd-area
boards with answer 0.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1)
elif seed == 2:
    print(9, 9)
elif seed == 3:
    print(8, 9)
elif seed == 4:
    print(2, 3)
elif seed == 5:
    print(1, 9)
else:
    print(rng.randint(1, 9), rng.randint(1, 9))
