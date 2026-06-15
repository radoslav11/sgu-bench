"""Tests for p186 (The Chain): minutes to merge N chains into one.

N <= 100 chains, each length 1..100. Seed 1 is the minimal single chain,
seed 2 has 100 chains of length 100, seed 3 has 100 chains of length 1
(the sacrificed chains run out of links). Other seeds mix many short
chains with a few long ones and uniform random lengths.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(lengths):
    print(len(lengths))
    print(" ".join(map(str, lengths)))


if seed == 1:
    emit([1])
elif seed == 2:
    emit([100] * 100)
elif seed == 3:
    emit([1] * 100)
elif seed == 4:
    # one huge chain and 99 unit chains
    emit([100] + [1] * 99)
elif seed == 5:
    # two chains, smallest possible merge
    emit([1, 100])
else:
    n = rng.randint(1, 100)
    emit([rng.randint(1, 100) for _ in range(n)])
