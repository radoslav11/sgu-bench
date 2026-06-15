"""Tests for p377 (The Lesson of Physical Culture): 2 <= N, M <= 1000.

Seed 1 minimal, seed 2 maximal square, seed 3 maximal skewed (min x max),
later seeds random plus equal-dimension cases.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(2, 2)
elif seed == 2:
    print(1000, 1000)
elif seed == 3:
    print(2, 1000)
elif seed == 4:
    print(1000, 2)
elif seed == 5:
    x = rng.randint(2, 1000)
    print(x, x)
else:
    print(rng.randint(2, 1000), rng.randint(2, 1000))
