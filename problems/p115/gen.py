"""Tests for p115 (Calendar): two positive integers N, M <= 100.

Seed 1 is January 1st, seed 2 the maximal pair (100, 100), seed 3 the
last valid date (31, 12), seeds 4-6 invalid-date edges (31/2, 29/2 in
the non-leap 2001, 31/4), the rest random pairs in 1..100 (mostly
valid-ish small values mixed with out-of-range ones).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1)
elif seed == 2:
    print(100, 100)
elif seed == 3:
    print(31, 12)
elif seed == 4:
    print(31, 2)
elif seed == 5:
    print(29, 2)
elif seed == 6:
    print(31, 4)
elif seed <= 8:
    print(rng.randint(1, 31), rng.randint(1, 12))
else:
    print(rng.randint(1, 100), rng.randint(1, 100))
