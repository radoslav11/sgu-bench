"""Tests for p306 (Balance): single integer n, 3 <= n <= 100.

Seed 1 is minimal (n=3), seeds 2-3 maximal (n=100, 99). Seeds 4-5 sit on
the (3^k - 1) / 2 capacity boundary (40 and 41 need 4 and 5 weighings
respectively); the rest are random mid-size values.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

fixed = {1: 3, 2: 100, 3: 99, 4: 40, 5: 41, 6: 13, 7: 4}
print(fixed.get(seed, rng.randint(5, 98)))
