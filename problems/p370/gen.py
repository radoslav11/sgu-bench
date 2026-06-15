"""Tests for p370 (Rifleman): count lines through the corner of an NxM grid.

Input is a single pair 1 <= N, M <= 10^6. Seed 1 is the 1x1 board, seeds
2-3 are maximal (TLE bait for O(N*M) pair enumeration), the rest cover
degenerate single-row/column boards and random sizes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAX = 10**6

if seed == 1:
    print(1, 1)
elif seed == 2:
    print(MAX, MAX)
elif seed == 3:
    print(MAX, 999983)
elif seed == 4:
    print(1, MAX)
elif seed == 5:
    print(MAX, 1)
elif seed == 6:
    print(2, 2)
else:
    print(rng.randint(1, MAX), rng.randint(1, MAX))
