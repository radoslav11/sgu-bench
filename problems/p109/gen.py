"""Tests for p109 (Magic of David Copperfield II): single N, 1 < N < 101.

Seed 1 is the minimal N = 2, seeds 2-3 the maximal N = 100 and 99
(even/odd parity of the grid side), the rest sweep random middle sizes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(2)
elif seed == 2:
    print(100)
elif seed == 3:
    print(99)
elif seed == 4:
    print(3)
else:
    print(rng.randint(4, 98))
