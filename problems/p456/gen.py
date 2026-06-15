"""Tests for p456 (Annuity Payment Scheme): 1 <= s <= 10^6, 1 <= m <= 120,
0 <= p <= 100.

Seed 1 is minimal, seeds 2-3 hit the maximal corners (p = 100 and the
p = 0 special branch at full s, m), seed 4 is the statement sample,
later seeds cover p = 0 / p = 100 / m = 1 edges and random mixes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1, 0)
elif seed == 2:
    print(10**6, 120, 100)
elif seed == 3:
    print(10**6, 120, 0)
elif seed == 4:
    print(100, 2, 50)
elif seed == 5:
    print(10**6, 120, 1)
elif seed == 6:
    print(rng.randint(1, 10**6), 1, rng.randint(0, 100))
elif seed == 7:
    print(rng.randint(1, 10**6), rng.randint(1, 120), 100)
else:
    print(rng.randint(1, 10**6), rng.randint(1, 120), rng.randint(0, 100))
