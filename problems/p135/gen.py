"""Tests for p135 (Drawing Lines): max zones from N lines, 0 <= N <= 65535.

Seed 1 is N=0, seed 2 the maximum N=65535, seed 3 N=65534, the rest
small and random values.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(0)
elif seed == 2:
    print(65535)
elif seed == 3:
    print(65534)
elif seed == 4:
    print(1)
elif seed == 5:
    print(2)
else:
    print(rng.randint(3, 65533))
