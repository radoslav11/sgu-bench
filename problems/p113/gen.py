"""Tests for p113 (Nearly prime numbers): 1 <= N <= 10, values <= 1e9.

Seed 1 is the single minimal value 1, seed 2 ten primes near 1e9 (worst
case for trial division, all "No"), seed 3 ten near-1e9 semiprimes and
prime squares (all "Yes"), seed 4 small edge values (1, 2, 4, 6, 8, 9),
the rest random mixes of primes, semiprimes and composites.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def is_prime(x):
    if x < 2:
        return False
    d = 2
    while d * d <= x:
        if x % d == 0:
            return False
        d += 1
    return True


def next_prime(x):
    while not is_prime(x):
        x += 1
    return x


def emit(vals):
    print(len(vals))
    print("\n".join(map(str, vals)))


if seed == 1:
    emit([1])
elif seed == 2:
    emit([next_prime(rng.randint(999000000, 999999000)) for _ in range(10)])
elif seed == 3:
    vals = []
    for i in range(10):
        if i < 2:
            p = next_prime(rng.randint(31000, 31600))
            vals.append(p * p)
        else:
            p = next_prime(rng.randint(20000, 31000))
            q = next_prime(rng.randint(20000, 1000000000 // p - 1000))
            vals.append(p * q)
    emit(vals)
elif seed == 4:
    emit([1, 2, 4, 6, 8, 9])
else:
    vals = []
    for _ in range(rng.randint(1, 10)):
        kind = rng.randint(0, 3)
        if kind == 0:
            vals.append(next_prime(rng.randint(2, 10**9 - 100)))
        elif kind == 1:
            p = next_prime(rng.randint(2, 30000))
            q = next_prime(rng.randint(2, 10**9 // p - 100))
            vals.append(p * q)
        elif kind == 2:
            vals.append(rng.randint(1, 10**9))
        else:
            vals.append(8 * next_prime(rng.randint(2, 10**8)))
    emit(vals)
