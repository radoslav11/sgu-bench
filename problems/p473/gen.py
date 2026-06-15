"""Tests for p473 (Droid formation): one number K, 1 <= K <= 10^5.

Seed 1 is the minimal K = 1. Seed 2 is the heaviest output case,
K = 99991 (the largest prime below 10^5, answer 2^99990 with ~30000
digits). Seed 3 is the maximal composite K = 100000 with the largest
factorization enumeration. Later seeds hit powers of two, smooth
numbers with many factorizations, and random values.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

fixed = {
    1: 1,
    2: 99991,
    3: 100000,
    4: 65536,
    5: 98304,
    6: 83160,
    7: 2,
}

if seed in fixed:
    print(fixed[seed])
elif seed == 8:
    # a random large prime-ish odd K
    print(rng.choice([99989, 99971, 99961, 99877]))
else:
    print(rng.randint(2, 100000))
