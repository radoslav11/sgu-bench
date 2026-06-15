"""Tests for p443 (Everlasting...?): compare key numbers of a and b.

key(n) = 2 * max(distinct primes of n) - sum(distinct primes of n),
2 <= a, b <= 10^6, key(a) != key(b) guaranteed. Seed 1 is minimal,
seeds 2-3 stress the largest values (factorization near 10^6), the
rest mix primes, prime powers, smooth and random numbers.
"""

import random
import sys


def key(n):
    s = 0
    mx = 0
    i = 2
    while i * i <= n:
        if n % i == 0:
            s += i
            mx = i
            while n % i == 0:
                n //= i
        i += 1
    if n > 1:
        s += n
        mx = n
    return 2 * mx - s


def emit(a, b):
    assert 2 <= a <= 10**6 and 2 <= b <= 10**6 and key(a) != key(b)
    print(a, b)


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit(2, 3)
elif seed == 2:
    # largest allowed values: 999998 = 2*31*127^2, 10^6 = 2^6*5^6
    emit(999998, 1000000)
elif seed == 3:
    # two large primes close to the limit
    emit(999983, 999979)
elif seed == 4:
    # smooth number vs prime power
    emit(2 * 3 * 5 * 7 * 11 * 13 * 17, 2**19)
else:
    pools = [
        lambda: rng.randint(2, 10**6),
        lambda: rng.randint(2, 10**6),
        lambda: rng.randint(10**6 - 1000, 10**6),
        lambda: 2 ** rng.randint(1, 19),
        lambda: 6 * rng.randint(1, 166666),
    ]
    while True:
        a = rng.choice(pools)()
        b = rng.choice(pools)()
        if key(a) != key(b):
            emit(a, b)
            break
