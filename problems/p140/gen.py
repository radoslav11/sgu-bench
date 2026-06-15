"""Tests for p140 (Integer Sequences): N <= 100, P <= 10000, A_i < 2e9.

Seed 1 is the minimal degenerate case (N=1, P=1). Seeds 2-3 are maximal
(N=100): seed 2 random values (gcd almost surely 1, answer YES), seed 3
all A_i sharing a divisor d of P while d does not divide B (answer NO).
Seeds 4-5 are the all-zero sequence with B=0 / B>0. The rest are random
mixes, half of them biased toward small common divisors.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXA = 1999999999

if seed == 1:
    n, p, b = 1, 1, 0
    a = [0]
elif seed == 2:
    n, p, b = 100, 10000, rng.randint(0, 9999)
    a = [rng.randint(0, MAXA) for _ in range(n)]
elif seed == 3:
    n, p, b = 100, 10000, 5  # d = 10 divides P and every A_i, not B
    a = [10 * rng.randint(0, MAXA // 10) for _ in range(n)]
elif seed == 4:
    n, p, b = 100, 10000, 0
    a = [0] * n
elif seed == 5:
    n, p, b = 100, 10000, 7
    a = [0] * n
else:
    n = rng.randint(1, 100)
    p = rng.randint(1, 10000)
    b = rng.randint(0, p - 1)
    if seed % 2 == 0:
        d = rng.choice([2, 3, 4, 5, 6, 8])
        a = [d * rng.randint(0, MAXA // d) for _ in range(n)]
    else:
        a = [rng.randint(0, MAXA) for _ in range(n)]

print(n, p, b)
print(" ".join(map(str, a)))
