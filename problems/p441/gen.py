"""Tests for p441 (Set Division): Stirling numbers S(N, K) mod 2007.

The statement's numeric bounds are lost; tests use 1 <= N <= 10^18 and
1 <= K <= 30 (the reference reads N as int64 and runs O(K^3 log N)
matrix exponentiation). Seed 1 is the minimal case, seeds 2-3 the
maximal ones (N = 10^18 with K = 30). Other seeds cover N = K, K = 1,
N < K (answer 0) and random pairs across magnitudes of N.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1)
elif seed == 2:
    print(10**18, 30)
elif seed == 3:
    print(10**18 - 1, 29)
elif seed == 4:
    print(30, 30)
elif seed == 5:
    print(10**18, 1)
elif seed == 6:
    print(5, 30)
elif seed == 7:
    print(rng.randint(1, 1000), rng.randint(1, 30))
else:
    n = rng.randint(1, 10**rng.randint(1, 18))
    k = rng.randint(1, 30)
    print(n, k)
