"""Tests for p127 (Telephone directory): 0 < K < 255, 0 < N < 8000.

Phone numbers are 4 digits, distinct, first digit in 1-7 or 9 (not 0
or 8) -- exactly 8000 valid numbers, so N = 7999 uses almost all of
them. Seed 1 is minimal, seed 2 maximal with K = 254, seed 3 maximal
with K = 1, the rest random subsets with random K.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

ALL_NUMBERS = [
    f"{d}{rest:03d}" for d in (1, 2, 3, 4, 5, 6, 7, 9) for rest in range(1000)
]

if seed == 1:
    k, n = 1, 1
elif seed == 2:
    k, n = 254, 7999
elif seed == 3:
    k, n = 1, 7999
else:
    k = rng.randint(1, 254)
    n = rng.randint(1, 7999)

numbers = rng.sample(ALL_NUMBERS, n)
print(k)
print(n)
print("\n".join(numbers))
