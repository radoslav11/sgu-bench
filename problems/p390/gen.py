"""Tests for p390 (Tickets): 1 <= l <= r <= 10^18, 1 <= k <= 1000.

Seed 1 is the minimal input, seeds 2-3 are maximal full-range cases with
extreme k, the rest mix random ranges, narrow ranges near digit-sum
boundaries and k values around the typical per-ticket digit sum.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAX = 10**18

if seed == 1:
    print(1, 1, 1)
elif seed == 2:
    print(1, MAX, 1000)
elif seed == 3:
    print(1, MAX, 1)
elif seed == 4:
    # Full range with k near the average digit sum of an 18-digit number,
    # which maximizes leftover discarding at passenger boundaries.
    print(1, MAX, 81)
elif seed == 5:
    # High range only: numbers close to 10^18 with large k.
    l = MAX - rng.randint(10**15, 10**16)
    print(l, MAX, rng.randint(900, 1000))
elif seed == 6:
    # Range around a long run of trailing nines (digit-sum spikes).
    base = 10**17 * 9 + 10**9 - 1
    print(base - 10**6, base + 10**6, rng.randint(100, 200))
elif seed == 7:
    # Single ticket, k larger than its digit sum: answer 0.
    x = rng.randint(10**17, MAX)
    print(x, x, 1000)
else:
    l = rng.randint(1, MAX)
    r = rng.randint(l, MAX)
    print(l, r, rng.randint(1, 1000))
