"""Tests for p159 (Self-Replicating Numbers): 2 <= b <= 36, 1 <= n <= 2000.

Seed 1 is the minimal b=2, n=1. Seed 2 is b=30 (three prime factors,
the most solutions: six) at n=2000; seed 3 is b=36 at n=2000. Seed 4
is b=2 at max length, seed 5 the decimal classic at max length, seed 6
a prime base at max length (zero solutions). The rest are random
bases/lengths, biased toward composite bases.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    b, n = 2, 1
elif seed == 2:
    b, n = 30, 2000
elif seed == 3:
    b, n = 36, 2000
elif seed == 4:
    b, n = 2, 2000
elif seed == 5:
    b, n = 10, 2000
elif seed == 6:
    b, n = 31, 2000
else:
    b = rng.choice([6, 10, 12, 15, 20, 24, 30, 33, 34, 35, 36, rng.randint(2, 36)])
    n = rng.choice([rng.randint(1, 30), rng.randint(30, 2000)])

print(b, n)
