"""Tests for p489 (Extremal Permutations): 1 <= n <= 1000, 1 <= m <= 1e9.

Seed 1 is the minimal n, seeds 2-3 are maximal n with different moduli, the
rest mix random sizes with adversarial moduli (1, prime, power of two).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1)
elif seed == 2:
    print(1000, 1000000000)
elif seed == 3:
    print(1000, 999999937)
elif seed == 4:
    print(1000, 1)
elif seed == 5:
    print(999, 2)
elif seed == 6:
    print(2, 1000000000)
else:
    n = rng.randint(1, 1000)
    m = rng.choice([
        rng.randint(1, 1000000000),
        1,
        2,
        1000000000,
        998244353,
        1000000007,
    ])
    print(n, m)
