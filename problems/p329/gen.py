"""Tests for p329 (Black-and-White Triangle): N <= 5, counts sum to N^2.

Seed 1 is the minimal N=1 case, seeds 2-3 are N=5 cases with different
count distributions, the rest are random N and random compositions of N^2
into four kinds (including skewed/all-one-kind patterns).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def random_composition(rng, total):
    cuts = sorted(rng.randint(0, total) for _ in range(3))
    return [
        cuts[0],
        cuts[1] - cuts[0],
        cuts[2] - cuts[1],
        total - cuts[2],
    ]


if seed == 1:
    n = 1
    counts = [0, 1, 0, 0]
elif seed == 2:
    n = 5
    counts = [7, 6, 6, 6]
elif seed == 3:
    n = 5
    counts = [0, 13, 12, 0]
elif seed == 4:
    n = 5
    counts = [0, 0, 0, 25]  # all of a single kind
elif seed == 5:
    n = 5
    counts = [25, 0, 0, 0]
else:
    n = rng.randint(1, 5)
    counts = random_composition(rng, n * n)

print(n)
print(*counts)
