"""Tests for p246 (Black & White): one odd integer L = 2N-1, 5 <= L < 2^31.

Seed 1 is the minimum L=5, seed 2 the maximum odd 2^31-1, seed 3 a huge L
on the other gcd branch (N = 2 mod 3). Remaining seeds cover both residue
classes of N mod 3 at random magnitudes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXL = 2**31 - 1

if seed == 1:
    print(5)
elif seed == 2:
    print(MAXL)  # N = 2^30, N % 3 == 1
elif seed == 3:
    # Largest odd L below max with N % 3 == 2 (gcd branch g = 3).
    L = MAXL
    while ((L + 1) // 2) % 3 != 2:
        L -= 2
    print(L)
elif seed == 4:
    print(7)
else:
    L = rng.randrange(5, MAXL, 2)
    print(L)
