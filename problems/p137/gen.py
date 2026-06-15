"""Tests for p137 (Funny Strings): N and K with 2 <= N <= 1000,
1 <= K <= 30000, gcd(N, K) = 1.

Seed 1 is the smallest case (2, 1), seed 2 the maximal (1000, 29999),
seed 3 another maximal shape (997, 30000, K > N with big quotient),
seed 4 K=1 with max N, seed 5 the sample; the rest are random coprime
pairs including K < N and K close to N.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(2, 1)
elif seed == 2:
    print(1000, 29999)
elif seed == 3:
    print(997, 30000)
elif seed == 4:
    print(1000, 1)
elif seed == 5:
    print(9, 16)
elif seed == 6:
    print(999, 1000)
else:
    while True:
        n = rng.randint(2, 1000)
        k = rng.randint(1, 30000)
        if math.gcd(n, k) == 1:
            print(n, k)
            break
