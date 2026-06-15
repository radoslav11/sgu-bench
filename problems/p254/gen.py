"""Tests for p254 (Strange Random): 1 <= N <= 2000000, 1 <= Q <= 10.

Seed 1 is the minimal input, seeds 2-3 use the maximal N with different Q,
the rest mix random and small adversarial cases.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1)
elif seed == 2:
    print(2000000, 10)
elif seed == 3:
    print(2000000, 1)
elif seed == 4:
    print(2000000, rng.randint(2, 9))
elif seed == 5:
    print(rng.randint(2, 20), rng.randint(1, 10))
elif seed == 6:
    print(rng.randint(1000, 100000), rng.randint(1, 10))
elif seed == 7:
    print(1999999, 10)
else:
    print(rng.randint(100000, 2000000), rng.randint(1, 10))
