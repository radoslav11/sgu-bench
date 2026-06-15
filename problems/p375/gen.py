"""Tests for p375 (Amplifiers): single integer 1 <= N <= 2*10^9.

Seed 1 is minimal, seed 2 the maximal even value ("No solution"), seed 3
the maximal odd value (longest amplifier chain). Later seeds mix random
odd/even values and powers of two +-1.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1)
elif seed == 2:
    print(2000000000)
elif seed == 3:
    print(1999999999)
elif seed == 4:
    print(2)
elif seed == 5:
    print(2**30 + 1)
elif seed == 6:
    print(2**30 - 1)
elif seed == 7:
    print(3)
else:
    print(rng.randint(1, 2 * 10**9))
