"""Tests for p258 (Almost Lucky Numbers): one line with 0 <= A <= B <= 1e9.

Seed 1 is minimal (0 0). Seeds 2-3 cover the full range and the widest
even-digit stretch. Later seeds pick random ranges, ranges straddling
digit-length boundaries (99..100, 9999..10000, ...) and A=B points.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAX = 10**9

if seed == 1:
    print(0, 0)
elif seed == 2:
    print(0, MAX)
elif seed == 3:
    print(1, MAX - 1)
elif seed == 4:
    b = rng.choice([100, 10000, 10**6, 10**8])
    print(b - 1, b)
elif seed == 5:
    a = rng.randint(10**7, 10**8 - 1)
    print(a, a)
else:
    a = rng.randint(0, MAX)
    b = rng.randint(0, MAX)
    if a > b:
        a, b = b, a
    print(a, b)
