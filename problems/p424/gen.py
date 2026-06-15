"""Tests for p424 (Beautiful graph): single integer 1 <= n <= 100.

Seed 1 is the minimum, seeds 2-3 the two largest values, then the
small special cases (2, 3, 4) and random mid-range values.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1)
elif seed == 2:
    print(100)
elif seed == 3:
    print(99)
elif seed == 4:
    print(2)
elif seed == 5:
    print(3)
elif seed == 6:
    print(4)
elif seed == 7:
    print(5)
else:
    print(rng.randint(6, 98))
