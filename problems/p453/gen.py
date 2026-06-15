"""Tests for p453 (Meetings): l, v1, v2, t are integers in [1, 10^9].

Seed 1 is the minimal input, seeds 2-3 are maximal (all values near 1e9
with different speed relations), later seeds cover equal speeds, l = 1,
divisibility/parity edge cases for the overlap term, and random values.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

M = 10**9

if seed == 1:
    print(1, 1, 1, 1)
elif seed == 2:
    print(M, M, M - 1, M)
elif seed == 3:
    print(1, M, M, M)
elif seed == 4:
    # equal speeds (no catch-up meetings)
    v = rng.randint(1, M)
    print(rng.randint(1, M), v, v, rng.randint(1, M))
elif seed == 5:
    # speeds with odd s/g and d/g so the overlap branch triggers:
    # v1 = 2g, v2 = g gives s = 3g, d = g, s/g = 3, d/g = 1, both odd
    g = rng.randint(1, 10**8)
    v1, v2 = 2 * g, g
    print(rng.randint(1, M), v1, v2, rng.randint(1, M))
elif seed == 6:
    # large l relative to speeds and time: zero meetings possible
    print(M, 1, 1, rng.randint(1, 100))
elif seed == 7:
    # tiny l with huge speeds: maximal meeting counts
    print(1, M, M - 1, M)
else:
    print(
        rng.randint(1, M),
        rng.randint(1, M),
        rng.randint(1, M),
        rng.randint(1, M),
    )
