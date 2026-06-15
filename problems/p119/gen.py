"""Tests for p119 (Magic Pairs): positive N, A0, B0 <= 10000, output all
distinct (k*A0 mod N, k*B0 mod N).

Seed 1 is N = 1, seed 2 the maximal N = 10000 with A0, B0 coprime to N
(full 10000 pairs), seed 3 N = 9973 prime with A0, B0 multiples of N
(only the zero pair), seed 4 N = 10000 with gcd-sharing A0, B0,
seed 5 the sample, the rest random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, a0, b0):
    print(n)
    print(a0, b0)


if seed == 1:
    emit(1, 1, 1)
elif seed == 2:
    emit(10000, 9999, 7)
elif seed == 3:
    emit(9973, 9973, 9973)
elif seed == 4:
    emit(10000, 5000, 7500)
elif seed == 5:
    emit(3, 1, 2)
else:
    emit(rng.randint(2, 10000), rng.randint(1, 10000),
         rng.randint(1, 10000))
