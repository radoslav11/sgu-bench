"""Tests for p118 (Digital Root): K <= 5 cases, each N <= 1000 values
in [0, 1e9].

Seed 1 is one single-element case, seed 2 five maximal random cases,
seed 3 five maximal cases salted with zeros and multiples of nine,
seed 4 cases starting with 0 (whole expression is 0), seed 5 all
values multiples of 9 (digital root 9 path), the rest random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(cases):
    print(len(cases))
    for a in cases:
        print(len(a), " ".join(map(str, a)))


if seed == 1:
    emit([[1]])
elif seed == 2:
    emit([[rng.randint(1, 10**9) for _ in range(1000)] for _ in range(5)])
elif seed == 3:
    cases = []
    for _ in range(5):
        a = [rng.choice([0, 9, 999999999, 18, rng.randint(1, 10**9)])
             for _ in range(1000)]
        a[0] = rng.randint(1, 10**9)
        cases.append(a)
    emit(cases)
elif seed == 4:
    emit([[0] + [rng.randint(0, 10**9) for _ in range(99)],
          [0],
          [rng.randint(1, 10**9) for _ in range(50)]])
elif seed == 5:
    emit([[9 * rng.randint(1, 10**8) for _ in range(200)] for _ in range(5)])
else:
    cases = []
    for _ in range(rng.randint(1, 5)):
        n = rng.randint(1, 1000)
        cases.append([rng.randint(0, 10**9) for _ in range(n)])
    emit(cases)
