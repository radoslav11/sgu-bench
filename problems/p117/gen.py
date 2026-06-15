"""Tests for p117 (Counting): 0 < N, M, K < 10001, sequence values in
[1, 10001].

Seed 1 is the minimal single-element case, seed 2 the maximal
N = M = 10000 with K a highly composite 9240 (deep prime powers
matter), seed 3 maximal with K a prime and many multiples planted,
seed 4 K = 1 (everything divisible), seed 5 M = 1, the rest random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, m, k, vals):
    print(n, m, k)
    print(" ".join(map(str, vals)))


if seed == 1:
    emit(1, 1, 2, [2])
elif seed == 2:
    emit(10000, 10000, 9240,
         [rng.randint(1, 10001) for _ in range(10000)])
elif seed == 3:
    k = 9973
    vals = [rng.choice([k, 2 * k % 10002 + 1, rng.randint(1, 10001)])
            for _ in range(10000)]
    emit(10000, 10000, k, vals)
elif seed == 4:
    emit(100, 50, 1, [rng.randint(1, 10001) for _ in range(100)])
elif seed == 5:
    emit(5000, 1, rng.randint(2, 10000),
         [rng.randint(1, 10001) for _ in range(5000)])
else:
    n = rng.randint(1, 10000)
    m = rng.randint(1, 10000)
    k = rng.randint(2, 10000)
    emit(n, m, k, [rng.randint(1, 10001) for _ in range(n)])
