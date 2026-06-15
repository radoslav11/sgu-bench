"""Tests for p200 (Cracking RSA): t, m <= 100, each b_i <= 10^9 with all
prime factors among the first t primes.

Seed 1 is minimal (t=1, m=1). Seed 2 is t=100, m=100 with random smooth
numbers. Seed 3 is t=100, m=100 where every b_i is a perfect square, so
the answer is the 31-digit 2^100 - 1 (full big-integer stress). Seeds 4+
mix sizes, duplicates, ones, and small-t/large-m shapes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def first_primes(t):
    primes, p = [], 2
    while len(primes) < t:
        if all(p % q for q in primes):
            primes.append(p)
        p += 1
    return primes


def smooth(rng, primes, square=False):
    # random product of the given primes staying <= 10^9
    x = 1
    while True:
        p = rng.choice(primes)
        step = p * p if square else p
        if x * step > 10**9:
            break
        x *= step
        if rng.random() < 0.25:
            break
    return x


if seed == 1:
    print(1, 1)
    print(2)
    sys.exit()

if seed == 2:
    t, m = 100, 100
    primes = first_primes(t)
    b = [smooth(rng, primes) for _ in range(m)]
elif seed == 3:
    t, m = 100, 100
    primes = first_primes(t)
    b = [smooth(rng, primes, square=True) for _ in range(m)]
elif seed == 4:
    # small prime set, many numbers: low rank, big answer
    t, m = 3, 100
    primes = first_primes(t)
    b = [smooth(rng, primes) for _ in range(m)]
elif seed == 5:
    # all ones: rank 0, answer 2^100 - 1
    t, m = 100, 100
    b = [1] * m
elif seed == 6:
    # duplicated values force dependent rows
    t, m = 20, 100
    primes = first_primes(t)
    base = [smooth(rng, primes) for _ in range(10)]
    b = [rng.choice(base) for _ in range(m)]
else:
    t = rng.randint(1, 100)
    m = rng.randint(1, 100)
    primes = first_primes(t)
    b = [smooth(rng, primes) for _ in range(m)]

print(t, m)
print(" ".join(map(str, b)))
