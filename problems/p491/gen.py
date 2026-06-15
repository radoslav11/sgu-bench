"""Tests for p491 (Game for Little Johnny): count pairs (A,B), 1<=A<B<=N,
with A*x+B*y=N for some x,y>=1.

Seed 1 is the minimal N. Seeds 2-3 are maximal N with different shapes
(a highly composite value and a prime) to stress the divisor enumeration.
Later seeds mix mid-size N, primes, powers of two, and highly composite
numbers (more divisors -> heavier inner loop).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXN = 600000

if seed == 1:
    print(2)
elif seed == 2:
    # Maximal, highly composite (many divisors per remainder).
    print(MAXN)
elif seed == 3:
    # Maximal, a prime just below the limit (different divisor structure).
    print(599999)
elif seed == 4:
    # Power of two near the limit.
    print(524288)
elif seed == 5:
    # Highly composite number with many small factors.
    print(498960)
elif seed == 6:
    print(3)
else:
    choice = rng.randint(0, 2)
    if choice == 0:
        print(rng.randint(2, MAXN))
    elif choice == 1:
        # An even, factor-rich value.
        print(2 * rng.randint(1, MAXN // 2))
    else:
        # A smaller value scaled for variety.
        print(rng.randint(2, 200000))
