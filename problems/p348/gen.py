"""Tests for p348 (Twisting the Number): one integer n, 1 <= n <= 10^18.

Seed 1 is n = 1, seed 2 the absolute maximum 10^18, seed 3 the largest
all-ones value 2^60 - 1 (different bit structure at full length). Later
seeds cover powers of two, 2^k - 1, alternating-bit patterns and random
values of every magnitude.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXN = 10**18

if seed == 1:
    print(1)
elif seed == 2:
    print(MAXN)
elif seed == 3:
    print((1 << 60) - 1)
elif seed == 4:
    print(1 << 59)  # power of two at max bit length
elif seed == 5:
    # alternating bits 1010...10, 60 bits
    print(int("10" * 30, 2))
elif seed == 6:
    print(6)  # statement sample
else:
    kind = rng.random()
    if kind < 0.3:
        bits = rng.randint(1, 59)
        print(rng.randint(1 << (bits - 1), (1 << bits) - 1))
    elif kind < 0.5:
        k = rng.randint(1, 59)
        print((1 << k) + rng.randint(0, 1) * rng.randint(0, (1 << k) - 1))
    else:
        print(rng.randint(1, MAXN))
