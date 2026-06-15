"""Tests for p105 (Div 3): single integer 1 <= N <= 2^31 - 1.

Seed 1 is N = 1, seed 2 the maximal N = 2^31 - 1, seed 3 one below the
maximum (different residue mod 3), seeds 4-5 cover the remaining small
residues, the rest are random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAX = 2**31 - 1

if seed == 1:
    print(1)
elif seed == 2:
    print(MAX)
elif seed == 3:
    print(MAX - 1)
elif seed == 4:
    print(2)
elif seed == 5:
    print(3)
else:
    print(rng.randint(1, MAX))
