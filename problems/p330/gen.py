"""Tests for p330 (Numbers): 2 <= A < B <= 10^12.

Seed 1 is the minimal case (2, 3). Seed 2 uses two odd semiprimes with
~10^6 smallest factors (full sqrt scans, answer exists). Seed 3 spans the
whole even range, stressing the sequence-length bound. Later seeds cover
large primes (Impossible with a full scan), close odd pairs where the
adjusted endpoints cross, and random mixes of parities and magnitudes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

BIG_PRIME = 999999999989  # largest prime below 10^12
P, Q = 999983, 999979  # primes near 10^6

if seed == 1:
    a, b = 2, 3
elif seed == 2:
    a, b = P * Q, P * P  # odd semiprimes, ~10^6 trial divisions each
elif seed == 3:
    a, b = 4, 999999999998  # even-to-even across the full range
elif seed == 4:
    a, b = BIG_PRIME - 100, BIG_PRIME  # B is a huge odd prime
elif seed == 5:
    a, b = 15, 21  # adjusted_A = 18 > adjusted_B = 14: Impossible
elif seed == 6:
    a, b = 2, 10**12  # no move from 2 at all
elif seed == 7:
    a, b = P * Q, P * Q + 2  # huge odd semiprimes, tiny gap
else:
    lo = rng.choice([2, 10, 10**6, 10**11])
    hi = rng.choice([10**3, 10**6, 10**12])
    if lo >= hi:
        lo, hi = 2, 10**12
    a = rng.randint(lo, hi - 1)
    b = rng.randint(a + 1, hi)

print(a, b)
