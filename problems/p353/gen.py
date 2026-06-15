"""Tests for p353 (Billing): six integers N, k1, k2, p1, p2, p3.

The exact bounds are mangled in the statement; we use 1..10^6 for every
value (consistent with the reference's 3*10^6 search cap). Seed 1 is
all-ones, seed 2 maximizes the answer (~3*10^6 seconds), seed 3 is all
values at 10^6. Later seeds target boundaries: N < p1, N == p1, exact
zero balance at each phase end, and a leftover positive balance smaller
than the per-second price (the "one extra second" rule).
"""

import random
import sys

MAXV = 10**6

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    case = (1, 1, 1, 1, 1, 1)
elif seed == 2:
    case = (MAXV, MAXV, MAXV, 1, 1, 1)
elif seed == 3:
    case = (MAXV, MAXV, MAXV, MAXV, MAXV, MAXV)
elif seed == 4:
    # N < p1: call cannot be established.
    p1 = rng.randint(2, MAXV)
    case = (rng.randint(1, p1 - 1), rng.randint(1, MAXV),
            rng.randint(1, MAXV), p1, rng.randint(1, MAXV),
            rng.randint(1, MAXV))
elif seed == 5:
    # N == p1: zero balance right after the connection fee.
    p1 = rng.randint(1, MAXV)
    case = (p1, rng.randint(1, MAXV), rng.randint(1, MAXV), p1,
            rng.randint(1, MAXV), rng.randint(1, MAXV))
elif seed == 6:
    # Balance runs out exactly at the end of the p2 segment.
    k2 = rng.randint(1, 1000)
    p2 = rng.randint(1, 1000)
    p1 = rng.randint(1, 1000)
    case = (p1 + k2 * p2, rng.randint(1, MAXV), k2, p1, p2,
            rng.randint(1, MAXV))
elif seed == 7:
    # Positive leftover smaller than p3: one extra second granted.
    p1, p2, p3 = 10, 10, rng.randint(2, MAXV)
    case = (p1 + 3 * p2 + 2 * p3 + 1, 5, 3, p1, p2, p3)
else:
    case = tuple(rng.randint(1, MAXV) for _ in range(6))

print(*case)
