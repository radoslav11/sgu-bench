"""Tests for p385 (Highlander): one integer n, 2 <= n <= 100.

Seed 1 is the minimal n, seeds 2-3 the two largest values (the whole
input space is just 99 integers), the rest are spread over the range.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(2)
elif seed == 2:
    print(100)
elif seed == 3:
    print(99)
elif seed == 4:
    print(3)
else:
    print(rng.randint(4, 98))
