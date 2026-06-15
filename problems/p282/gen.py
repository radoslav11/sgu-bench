"""Tests for p282 (Isomorphism): N <= 53, M <= 1000, prime N < P <= 10^9.

Seed 1 is minimal (1 1 2), seeds 2-3 maximal N=53 M=1000 with a huge and
the smallest allowed prime, the rest are random triples with random
primes (Miller-Rabin tested).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def is_prime(x):
    if x < 2:
        return False
    for p in (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37):
        if x % p == 0:
            return x == p
    d, s = x - 1, 0
    while d % 2 == 0:
        d //= 2
        s += 1
    for a in (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37):
        v = pow(a, d, x)
        if v in (1, x - 1):
            continue
        for _ in range(s - 1):
            v = v * v % x
            if v == x - 1:
                break
        else:
            return False
    return True


def random_prime(low, high):
    while True:
        x = rng.randint(low, high)
        if is_prime(x):
            return x


if seed == 1:
    print(1, 1, 2)
elif seed == 2:
    print(53, 1000, 999999937)
elif seed == 3:
    print(53, 1000, 59)
elif seed == 4:
    print(52, 999, random_prime(53, 10**9))
elif seed == 5:
    n = rng.randint(40, 53)
    print(n, rng.randint(2, 1000), random_prime(n + 1, 1000))
else:
    n = rng.randint(1, 53)
    m = rng.randint(1, 1000)
    print(n, m, random_prime(n + 1, 10**9))
