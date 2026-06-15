"""Tests for p249 (Matrix): just two integers N, M with N, M > 0 and
N + M <= 20; the answer is a 2^N x 2^M cyclic Gray-code matrix.

Seed 1 is the minimal 1 1, seeds 2-3 maximal N+M=20 with balanced and
extreme splits, the rest cover other sizes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1)
elif seed == 2:
    print(10, 10)
elif seed == 3:
    print(1, 19)
elif seed == 4:
    print(19, 1)
elif seed == 5:
    print(5, 15)
elif seed == 6:
    print(2, 2)
else:
    total = rng.randint(2, 18)
    n = rng.randint(1, total - 1)
    print(n, total - n)
