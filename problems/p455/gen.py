"""Tests for p455 (Sequence analysis): one line with int64 A, B (>0), C (>0).

Seed 1 is the minimal input, seeds 2-3 use full-magnitude A, B, C so the
orbit stays long (answer -1, maximal iteration count). Seeds 4-6 are the
statement samples (overflow and negative-A edge cases), seeds 7-8 use C
around 10^12-10^13 so rho ~ sqrt(C) lands the answer near the 2e6 index
cutoff, the rest mix random magnitudes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)
I64 = 1 << 63

if seed == 1:
    print("0 1 1")
elif seed == 2:
    print("6364136223846793005 9223372036854775807 9223372036854775807")
elif seed == 3:
    print("-6364136223846793005 1 9223372036854775783")
elif seed == 4:
    print("2305843009213693951 1 9223372036854775807")
elif seed == 5:
    print("-2 1 5")
elif seed == 6:
    print("2 2 9")
elif seed == 13:
    print("555555555555 7 800000000000")
elif seed == 7:
    print(rng.randint(-I64, I64 - 1), 1, 4 * 10**12 + 1)
elif seed == 8:
    print(
        rng.randint(-I64, I64 - 1),
        rng.randint(1, 10),
        rng.randint(10**12, 2 * 10**13),
    )
else:
    A = rng.randint(-I64, I64 - 1)
    B = rng.choice([1, rng.randint(1, 100), rng.randint(1, I64 - 1)])
    C = rng.choice(
        [rng.randint(1, 1000), rng.randint(1, 10**9), rng.randint(1, I64 - 1)]
    )
    print(A, B, C)
