"""Tests for p294 (He's Circles): single integer 1 <= n <= 200000.

Seed 1 is minimal, seed 2 maximal, seed 3 a maximal prime (only two
divisors, so the per-shift gcd grouping degenerates differently), then
a highly composite n, tiny values, and random mid-size values.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1)
elif seed == 2:
    print(200000)
elif seed == 3:
    print(199999)  # prime
elif seed == 4:
    print(166320)  # highly composite
elif seed == 5:
    print(rng.randint(2, 10))
else:
    print(rng.randint(2, 200000))
