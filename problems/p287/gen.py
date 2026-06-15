"""Tests for p287 (Amusing Qc Machine): one line "q c" with
1 <= q <= 10^15 and 0 <= c <= 10^6.

Seed 1 is the minimal input, seed 2 the maximal one (q = 10^15 with the
largest delay c = 10^6, which makes the answer and the work both peak at
roughly 3.2 million steps), seed 3 keeps q maximal with c = 0 (pure
binary search). Later seeds mix exact powers of two minus/plus one with
c = 0, q = 1 with a huge delay, and random (q, c) pairs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 0)
elif seed == 2:
    print(10**15, 10**6)
elif seed == 3:
    print(10**15, 0)
elif seed == 4:
    print(1, 10**6)
elif seed == 5:
    # threshold pair for c = 0: 2^n - 1 is doable in n steps, +1 is not
    print(2**40 - 1, 0)
elif seed == 6:
    print(2**40, 0)
elif seed == 7:
    print(rng.randint(1, 10**15), rng.randint(900000, 10**6))
else:
    print(rng.randint(1, 10**15), rng.randint(0, 10**6))
