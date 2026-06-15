"""Tests for p404 (Fortune-telling with camomile).

1 <= N, M <= 100, phrases are 1-100 latin letters. Seed 1 is minimal,
seeds 2-3 maximal (100 phrases of length 100), the rest random with
small/duplicate phrases and N < M / N = M edge patterns.
"""

import random
import string
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def word(length):
    return "".join(rng.choice(string.ascii_letters) for _ in range(length))


if seed == 1:
    print(1, 1)
    print("a")
elif seed in (2, 3):
    n, m = (100, 100) if seed == 2 else (1, 100)
    print(n, m)
    for _ in range(m):
        print(word(100))
else:
    n = rng.randint(1, 100)
    m = rng.randint(1, 100)
    print(n, m)
    for _ in range(m):
        print(word(rng.randint(1, 100)))
