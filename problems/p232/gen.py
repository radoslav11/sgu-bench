"""Tests for p232 (Infinite Fraction): best rotation over step-K cycles.

N <= 150000 digits, 0 <= K <= 10^9. Seed 2 is one full-length cycle
(K coprime to N) over random digits; seed 3 keeps N maximal but uses a
two-digit alphabet with long runs, the worst case for rotation
comparison. Other seeds cover K = 0, K a multiple of N, huge K with a
large gcd (many short cycles), all-equal digits, and random cases.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

NMAX = 150000


def emit(n, k, digits):
    print(n, k)
    print("".join(map(str, digits)))


if seed == 1:
    emit(1, 0, [rng.randint(0, 9)])
elif seed == 2:
    emit(NMAX, 7, [rng.randint(0, 9) for _ in range(NMAX)])
elif seed == 3:
    digits = []
    while len(digits) < NMAX:
        digits.extend([rng.choice([9, 8])] * rng.randint(1, 50))
    emit(NMAX, 149999, digits[:NMAX])
elif seed == 4:
    emit(NMAX, 0, [rng.randint(0, 9) for _ in range(NMAX)])
elif seed == 5:
    # K = 10^9: K mod N = 100000, gcd = 50000 -> 50000 cycles of length 3
    emit(NMAX, 10**9, [rng.choice([9, 8, 7]) for _ in range(NMAX)])
elif seed == 6:
    emit(NMAX, 150000, [9] * NMAX)  # K mod N == 0, all digits equal
elif seed == 7:
    emit(NMAX, 75000, [rng.randint(0, 9) for _ in range(NMAX)])
else:
    n = rng.randint(2, 5000)
    k = rng.randint(0, 10**9)
    emit(n, k, [rng.randint(0, 4) for _ in range(n)])
