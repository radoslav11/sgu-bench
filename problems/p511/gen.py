"""Tests for p511 (Fermat's Last Theorem modulo p).

t test cases, 1 <= t <= 1000. Each case is two integers n, p with
3 <= n <= 1e6 and p a prime, 2 <= p <= 1e6. We restrict p to odd primes
(>= 3): for p = 2 the reference's primitive_root returns -1 and indexes a
buffer at a negative position, so we avoid that degenerate input.

Seed 1 is the minimal case (t = 1, smallest n and p). Seed 2 is maximal:
t = 1000 cases all at the largest primes near 1e6 with n near 1e6 -- this is
the slowest input for the reference's primitive-root and subgroup-walk loops.
Seed 3 is a second max-size test using a spread of large primes and varied n
to exercise both the solution-found and no-solution branches at full size.
Later seeds mix small primes (many -1 answers), repeated primes, and random
values.
"""

import random
import sys

NMAX = 10**6
PMAX = 10**6

seed = int(sys.argv[1])
rng = random.Random(seed)


def sieve_primes(limit):
    is_p = bytearray([1]) * (limit + 1)
    is_p[0] = is_p[1] = 0
    for i in range(2, int(limit ** 0.5) + 1):
        if is_p[i]:
            for j in range(i * i, limit + 1, i):
                is_p[j] = 0
    return [i for i in range(3, limit + 1) if is_p[i]]  # odd primes only


PRIMES = sieve_primes(PMAX)


def emit(cases):
    out = [str(len(cases))]
    for n, p in cases:
        out.append(f"{n} {p}")
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    emit([(3, 3)])
    sys.exit()

if seed == 2:
    # maximal: many cases at the largest primes, n near max
    large = PRIMES[-200:]  # the 200 largest primes <= 1e6
    cases = []
    for _ in range(1000):
        p = rng.choice(large)
        n = rng.randint(NMAX - 1000, NMAX)
        cases.append((n, p))
    emit(cases)
    sys.exit()

if seed == 3:
    # second max-size: spread of large primes plus varied n distributions
    large = PRIMES[-50000:]
    cases = []
    for _ in range(1000):
        p = rng.choice(large)
        # mix: some n coprime-ish to p-1, some sharing factors -> different branches
        choice = rng.random()
        if choice < 0.5:
            n = rng.randint(3, NMAX)
        else:
            n = rng.choice([3, 4, 5, 6, p - 1 if p - 1 <= NMAX else 3, NMAX])
            n = max(3, min(NMAX, n))
        cases.append((n, p))
    emit(cases)
    sys.exit()

if seed == 4:
    # many small primes -> exercises lots of -1 / quick cases
    small = [p for p in PRIMES if p <= 200]
    cases = [(rng.randint(3, NMAX), rng.choice(small)) for _ in range(1000)]
    emit(cases)
    sys.exit()

if seed == 5:
    # repeated identical case (stress same primitive root / subgroup repeatedly)
    p = rng.choice(PRIMES[-100:])
    n = rng.randint(3, NMAX)
    emit([(n, p)] * 1000)
    sys.exit()

# seeds 6+: random mid-size mixtures
t = rng.randint(1, 1000)
cases = [(rng.randint(3, NMAX), rng.choice(PRIMES)) for _ in range(t)]
emit(cases)
