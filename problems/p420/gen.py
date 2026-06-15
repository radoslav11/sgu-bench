"""Tests for p420 (Number Permutations): one line with l r, 1 <= l <= r <= 1e15.

Seed 1 is the minimal segment, seeds 2-3 the full range / a huge
random-end segment, later seeds stress boundaries straddling digit-length
changes, l == r, and random segments of various widths.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LIM = 10 ** 15

if seed == 1:
    print(1, 1)
elif seed == 2:
    print(1, LIM)
elif seed == 3:
    print(rng.randint(1, 10 ** 14), LIM)
elif seed == 4:
    # segment straddling a power of ten
    p = 10 ** rng.randint(2, 14)
    print(p - rng.randint(1, p // 2), p + rng.randint(0, p))
elif seed == 5:
    v = rng.randint(1, LIM)
    print(v, v)
elif seed == 6:
    # narrow segment deep inside 15-digit numbers
    l = rng.randint(10 ** 14, LIM - 1000)
    print(l, l + rng.randint(0, 1000))
else:
    l = rng.randint(1, LIM)
    r = rng.randint(l, LIM)
    print(l, r)
