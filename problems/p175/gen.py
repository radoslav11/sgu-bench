"""Tests for p175 (Encoding): 1 <= N <= 10^9, 1 <= q <= N.

Seed 1 is N=1. Seeds 2-3 are maximal N=10^9 with q at both ends. Seeds
4-5 hit exact powers of two (clean halving) and 2^30-1/2^30+1 odd-split
chains; the rest are random (N, q) pairs across magnitudes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    n, q = 1, 1
elif seed == 2:
    n, q = 10**9, 10**9
elif seed == 3:
    n, q = 10**9, 1
elif seed == 4:
    n = 2**30
    q = n // 2
elif seed == 5:
    n = 2**30 - 1
    q = n // 2 + 1
else:
    n = rng.randint(1, 10**9) if seed % 2 else rng.randint(1, 10**3)
    q = rng.randint(1, n)

print(n, q)
