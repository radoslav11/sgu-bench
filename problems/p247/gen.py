"""Tests for p247 (Difficult Choice): N tests (0 <= N <= 100), each an odd
prime P with 1 < P < 1000.

Seed 1 is the empty case N=0, seed 2 is 100 copies of the largest prime
997, seed 3 is 100 distinct large primes. Later seeds pick random primes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def sieve(limit):
    is_p = [True] * limit
    is_p[0] = is_p[1] = False
    for i in range(2, int(limit**0.5) + 1):
        if is_p[i]:
            for j in range(i * i, limit, i):
                is_p[j] = False
    return [i for i in range(limit) if is_p[i]]


primes = [p for p in sieve(1000) if p > 2]

if seed == 1:
    print(0)
elif seed == 2:
    print(100)
    for _ in range(100):
        print(997)
elif seed == 3:
    big = primes[-100:]
    print(len(big))
    for p in big:
        print(p)
elif seed == 4:
    print(2)
    print(3)
    print(5)
else:
    n = rng.randint(1, 100)
    print(n)
    for _ in range(n):
        print(rng.choice(primes))
