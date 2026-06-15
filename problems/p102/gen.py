"""Tests for p102 (Coprimes / Euler phi): 1 <= N <= 10^4.

Seed 1 is N=1, seed 2 the maximum N=10000, seed 3 the largest prime
below 10^4, then primes, prime powers and random values.
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
    print(9973)
elif seed == 4:
    print(8192)
elif seed == 5:
    print(2 * 3 * 5 * 7 * 11 * 13)
else:
    print(rng.randint(2, 10000))
