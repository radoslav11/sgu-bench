"""Tests for p197 (Nice Patterns Strike Back): one line "N M P" with
1 <= N <= 10^100 (decimal, possibly hundreds of bits), 1 <= M <= 5,
1 <= P <= 10000.

Seed 1 is minimal (1 1 1). Seeds 2-3 are maximal: N = 10^100 with M=5 and
two different moduli. Other seeds vary digit count of N, M, and P,
including M=1 (no 2x2 squares at all), P=1 (answer 0) and small N where
brute force over all colorings is checkable.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1, 1)
elif seed == 2:
    print("1" + "0" * 100, 5, 10000)
elif seed == 3:
    print("9" * 100, 5, 9973)
elif seed == 4:
    print(rng.randint(1, 10**100), 1, rng.randint(2, 10000))
elif seed == 5:
    print(rng.randint(1, 4), rng.randint(1, 5), rng.randint(2, 10000))
elif seed == 6:
    print(rng.randint(1, 10**100), rng.randint(2, 5), 1)
else:
    digits = rng.randint(1, 100)
    n = rng.randint(1, 10**digits)
    print(n, rng.randint(1, 5), rng.randint(1, 10000))
