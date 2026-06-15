"""Tests for p408 (Game with points): a single integer N, 0 <= N <= 1000.

Seed 1 is N=0, seeds 2-3 the maximal N=1000 and N=999 (the O(N^2) DP's
worst cases), seed 4 is N=1, the rest random N.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(0)
elif seed == 2:
    print(1000)
elif seed == 3:
    print(999)
elif seed == 4:
    print(1)
else:
    print(rng.randint(2, 998))
