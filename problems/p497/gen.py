"""Tests for p497 (Abelian Groups): number of abelian groups of order n.

Input is a single integer n with 1 <= n <= 10^12. The answer is the
product of p(e) over prime powers p^e || n, where p(.) is the integer
partition function. The reference factors n by trial division up to
sqrt(n) ~ 10^6, so the heaviest cases are a large prime near 10^12 (full
trial loop) and the largest exponents (2^40, 3^25, ...).
"""

import random
import sys

N_MAX = 10**12

seed = int(sys.argv[1])
rng = random.Random(seed)

# Largest prime below 10^6 so that p^2 still fits the trial bound.
LARGE_PRIME = 999983  # prime
PRIME_NEAR_MAX = 999999999989  # a prime just below 10^12


if seed == 1:
    print(1)
elif seed == 2:
    # Large prime near the maximum: forces the full trial-division loop.
    print(PRIME_NEAR_MAX)
elif seed == 3:
    # Largest power of two below 10^12 => exponent 39, big partition factor.
    print(2**39)
elif seed == 4:
    # 3^25 ~ 8.5e11, another large single exponent.
    print(3**25)
elif seed == 5:
    # Highly composite: many distinct primes with assorted exponents.
    print(2**4 * 3**3 * 5**2 * 7 * 11 * 13 * 17)
elif seed == 6:
    # Product of two large primes (semiprime near max): worst trial loop
    # that still has a factor.
    print(LARGE_PRIME * 1000003)
elif seed == 7:
    # Mix a large power of two with another prime while staying in range:
    # 2^38 * 3 ~ 8.2e11.
    print(2**38 * 3)
else:
    # Random n in range, occasionally a perfect prime power.
    choice = rng.random()
    if choice < 0.3:
        p = rng.choice([2, 3, 5, 7, 11, 13])
        e = 1
        val = p
        while val * p <= N_MAX:
            val *= p
            e += 1
        e = rng.randint(1, e)
        print(p**e)
    else:
        print(rng.randint(1, N_MAX))
