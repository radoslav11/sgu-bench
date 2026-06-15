"""Tests for p261 (Discrete Roots): primes 2 <= P <= 1e9,
2 <= K <= 1e5, 0 <= A < P; find all x with x^K = A (mod P).

Seed 1 is minimal (P=2). Seed 2 is maximal with K | P-1 and A a K-th
power, so there are K=99991 roots (heavy output + heavy root
enumeration). Seed 3 is maximal with gcd(K, P-1)=1 (single root,
BSGS-heavy P near 1e9). Seeds 4-5 cover A=0 and a quadratic
non-residue (zero roots). The rest use random primes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def is_prime(n):
    if n < 2:
        return False
    for p in (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37):
        if n % p == 0:
            return n == p
    d, s = n - 1, 0
    while d % 2 == 0:
        d //= 2
        s += 1
    for a in (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37):
        x = pow(a, d, n)
        if x in (1, n - 1):
            continue
        for _ in range(s - 1):
            x = x * x % n
            if x == n - 1:
                break
        else:
            return False
    return True


def rand_prime(lo, hi):
    while True:
        c = rng.randint(lo, hi)
        if is_prime(c):
            return c


SMALL_PRIMES = [p for p in range(2, 1000) if is_prime(p)]

if seed == 1:
    print(2, 2, 1)
elif seed == 2:
    k = 99991
    p = (10**9 // k) * k + 1
    while not is_prime(p):
        p -= k
    a = pow(rng.randint(2, p - 1), k, p)
    print(p, k, a)
elif seed == 3:
    p = 999999937
    k = 99991
    assert (p - 1) % k != 0
    print(p, k, rng.randint(1, p - 1))
elif seed == 4:
    print(999999937, 2, 0)
elif seed == 5:
    p = 999999937
    while True:
        a = rng.randint(2, p - 1)
        if pow(a, (p - 1) // 2, p) != 1:
            break
    print(p, 2, a)
else:
    p = rand_prime(2, 10**9) if rng.random() < 0.5 else rand_prime(2, 1000)
    k = rng.choice(SMALL_PRIMES) if rng.random() < 0.5 else rand_prime(2, 10**5)
    if rng.random() < 0.5:
        a = pow(rng.randint(1, p - 1), k, p)
    else:
        a = rng.randint(0, p - 1)
    print(p, k, a)
