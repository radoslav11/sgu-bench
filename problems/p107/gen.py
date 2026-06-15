"""Tests for p107 (987654321 problem): single integer 1 <= N <= 1e6.

Seed 1 is N = 1, seed 2 the maximum N = 1e6, seed 3 another huge N,
seeds 4-6 the boundary values 8, 9, 10 around the answer's structure
change, the rest random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1)
elif seed == 2:
    print(10**6)
elif seed == 3:
    print(10**6 - 1)
elif seed == 4:
    print(8)
elif seed == 5:
    print(9)
elif seed == 6:
    print(10)
else:
    print(rng.randint(11, 10**6))
