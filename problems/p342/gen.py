"""Tests for p342 (Reihenfolge): A < 10^3000, 1 <= B <= 10^6.

Seed 1 is the smallest input (A=1, B=1). Seed 2 uses a full 3000-digit A
with B=2 (maximum number of base-B digits, ~10^4 division steps over a
big number). Seed 3 keeps 3000 digits with B=10^6 and digits engineered
around B-1 / carry chains. Other seeds cover B=1 with huge A, A = B^k
and B^k - 1 patterns, and random pairs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def rand_digits(n):
    return str(rng.randint(1, 9)) + "".join(str(rng.randint(0, 9)) for _ in range(n - 1))


if seed == 1:
    print(1)
    print(1)
elif seed == 2:
    print(rand_digits(3000))
    print(2)
elif seed == 3:
    # 3000 digits, B = 10^6, A built from base-B digits that are mostly
    # B-1 or 1 to force long carry/borrow chains in the DP.
    b = 10**6
    val = 0
    for _ in range(499):
        val = val * b + rng.choice([b - 1, b - 1, 1, 0, b // 2, b // 2 + 1])
    if val == 0:
        val = 1
    print(val)
    print(b)
elif seed == 4:
    # B = 1: answer is A itself (3000-digit output).
    print(rand_digits(3000))
    print(1)
elif seed == 5:
    # A = B^k for large k: answer 1.
    b = rng.randint(2, 10**6)
    k = 1
    while len(str(b ** (k + 1))) <= 3000:
        k += 1
    print(b**k)
    print(b)
elif seed == 6:
    # A = B^k - 1: alternating representation stress.
    b = rng.randint(2, 10**6)
    k = 1
    while len(str(b ** (k + 1))) <= 3000:
        k += 1
    print(b**k - 1)
    print(b)
elif seed == 7:
    # Small A, large B.
    print(rng.randint(1, 100))
    print(10**6)
else:
    n = rng.randint(1, 3000)
    print(rand_digits(n))
    print(rng.randint(1, 10**6))
