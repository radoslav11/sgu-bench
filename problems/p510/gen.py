"""Tests for p510 (Distinct Substrings): single integer n, 1 <= n <= 300.

The output is the shortest, lexicographically smallest string with exactly n
distinct substrings, so the input space is tiny. Seed 1 is the minimum (n=1),
seed 2 the maximum (n=300), seed 3 a second large value (n=299). Seeds 4-7
cover the special small cases the algorithm hard-codes (n=2,3,4,5). The
remaining seeds pick random n in range.
"""

import random
import sys

NMAX = 300

seed = int(sys.argv[1])
rng = random.Random(seed)

specials = {1: 1, 2: NMAX, 3: NMAX - 1, 4: 2, 5: 3, 6: 4, 7: 5}

if seed in specials:
    print(specials[seed])
else:
    print(rng.randint(1, NMAX))
