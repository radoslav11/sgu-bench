"""Tests for p495 (Kids and Prizes): expected number of prizes given out.

Input is a single line "N M" with 1 <= N, M <= 100000. The answer is
N * (1 - ((N-1)/N)^M). Seeds 1-3 hit the extreme corners; the rest are
random, with some structure (N >> M, M >> N, N == M).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

N_MAX = 100000
M_MAX = 100000

if seed == 1:
    print(1, 1)
elif seed == 2:
    # Both at the maximum.
    print(N_MAX, M_MAX)
elif seed == 3:
    # Many winners, few boxes: expectation saturates near N.
    print(2, M_MAX)
elif seed == 4:
    # Many boxes, few winners: nearly all picks give a fresh prize.
    print(N_MAX, 1)
elif seed == 5:
    print(N_MAX, N_MAX)
elif seed == 6:
    print(1, rng.randint(1, M_MAX))
elif seed == 7:
    n = rng.randint(1, N_MAX)
    print(n, n)
else:
    print(rng.randint(1, N_MAX), rng.randint(1, M_MAX))
