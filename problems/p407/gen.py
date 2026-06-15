"""Tests for p407 (Number of Paths in the Empire): one line "n m",
3 <= n <= 1000, 0 <= m <= 5000.

Seed 1 is minimal (n=3, m=0), seeds 2-3 maximal (n=1000/m=5000 and
n=3/m=5000 -- the answer has thousands of digits either way). Seeds 4-5
pin the m=0 and m=1 corner answers (1 and 0); the rest are random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(3, 0)
elif seed == 2:
    print(1000, 5000)
elif seed == 3:
    print(3, 5000)
elif seed == 4:
    print(rng.randint(3, 1000), 0)
elif seed == 5:
    print(rng.randint(3, 1000), 1)
else:
    print(rng.randint(3, 1000), rng.randint(2, 5000))
