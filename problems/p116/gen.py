"""Tests for p116 (Index of super-prime): one integer 1 <= N <= 10000.

Seed 1 is N = 1 (index 0), seed 2 the maximal N = 10000, seed 3
N = 9999, seed 4 N = 2 (smallest unrepresentable beyond 1), seed 5
N = 3 (a super-prime itself), seed 6 N = 4 (unrepresentable), the rest
random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1)
elif seed == 2:
    print(10000)
elif seed == 3:
    print(9999)
elif seed == 4:
    print(2)
elif seed == 5:
    print(3)
elif seed == 6:
    print(4)
else:
    print(rng.randint(5, 10000))
