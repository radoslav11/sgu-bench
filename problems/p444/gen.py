"""Tests for p444 (Headstrong Student): decimal expansion of x/y.

1 <= x, y <= 10^6. Output: digits before the recurring part and the
period length. Seed 1 is minimal, seed 2 a full-period prime divisor
near the limit, seed 3 a max divisor of the form 2^a*5^b*p (long
preperiod and long period). Others cover terminating fractions,
x divisible by y, y = 1, powers of 10, and random pairs.
"""

import random
import sys


def emit(x, y):
    assert 1 <= x <= 10**6 and 1 <= y <= 10**6
    print(x, y)


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit(1, 1)
elif seed == 2:
    # prime near the limit -> long-division loop of ~10^6 steps
    emit(1, 999983)
elif seed == 3:
    # 2^5 * 5^2 * 1249 = 999200: nonzero preperiod plus long period
    emit(999999, 999200)
elif seed == 4:
    # terminating fraction with the longest tail: y = 2^19
    emit(999999, 524288)
elif seed == 5:
    emit(1000000, 1000000)
else:
    kind = rng.randint(0, 3)
    x = rng.randint(1, 10**6)
    if kind == 0:
        y = rng.randint(1, 10**6)
    elif kind == 1:
        y = 2 ** rng.randint(0, 19)
        while y * 5 <= 10**6 and rng.random() < 0.5:
            y *= 5
    elif kind == 2:
        y = rng.choice([999983, 999979, 999961, 909091, 99990, 142857])
    else:
        y = rng.randint(1, 1000)
        x = y * rng.randint(1, 10**6 // y)
    emit(x, y)
