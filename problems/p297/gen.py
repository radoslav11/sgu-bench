"""Tests for p297 (Fair-play): N, M <= 100, bills S_i in [1, 10^4].

Seed 1 is minimal, seed 2 maximal with random bills, seed 3 maximal with
all bills at the 10^4 cap, then N=1 (everything divides), sums that are
exact multiples of N, and random cases.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, m, bills):
    print(n, m)
    for s in bills:
        assert 1 <= s <= 10**4
        print(s)


if seed == 1:
    emit(1, 1, [1])
elif seed == 2:
    emit(100, 100, [rng.randint(1, 10**4) for _ in range(100)])
elif seed == 3:
    emit(100, 100, [10**4] * 100)
elif seed == 4:
    emit(1, 100, [rng.randint(1, 10**4) for _ in range(100)])
elif seed == 5:
    # bills all divisible by N: answer 0
    emit(50, 100, [50 * rng.randint(1, 200) for _ in range(100)])
else:
    n = rng.randint(1, 100)
    m = rng.randint(1, 100)
    emit(n, m, [rng.randint(1, 10**4) for _ in range(m)])
