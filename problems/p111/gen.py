"""Tests for p111 (Very simple problem): integer 1 <= X <= 10^1000.

Seed 1 is X = 1, seed 2 the maximal X = 10^1000, seed 3 a random
1000-digit number, seeds 4-6 a 500-digit k squared exactly and the two
neighbours k^2 - 1, k^2 + 1 (floor boundary), the rest random lengths.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def rand_digits(length):
    first = rng.randint(1, 9)
    rest = "".join(str(rng.randint(0, 9)) for _ in range(length - 1))
    return int(str(first) + rest)


if seed == 1:
    print(1)
elif seed == 2:
    print(10**1000)
elif seed == 3:
    print(rand_digits(1000))
elif seed in (4, 5, 6):
    k = rand_digits(500)
    print(k * k + (seed - 5))
else:
    print(rand_digits(rng.randint(1, 999)))
