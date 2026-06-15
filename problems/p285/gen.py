"""Tests for p285 (What? Where? When?): N <= 1000 internet questions,
13 probabilities in [0,1] with at most 3 decimals.

Seed 1 is the all-zero degenerate game, seed 2 a maximal random test,
seed 3 maximal with only 0/1 probabilities, later seeds are random with
varied decimal precision.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def fmt(p_milli):
    # p_milli in [0, 1000] -> probability string with <= 3 decimals
    if p_milli == 1000:
        return "1"
    if p_milli == 0:
        return "0"
    return f"0.{p_milli:03d}"


def emit(sector_milli, internet_milli):
    print(len(internet_milli))
    print(" ".join(fmt(x) for x in sector_milli))
    print("\n".join(fmt(x) for x in internet_milli))


if seed == 1:
    emit([0] * 12, [0])
elif seed == 2:
    emit([rng.randint(0, 1000) for _ in range(12)],
         [rng.randint(0, 1000) for _ in range(1000)])
elif seed == 3:
    emit([rng.choice([0, 1000]) for _ in range(12)],
         [rng.choice([0, 1000]) for _ in range(1000)])
elif seed == 4:
    emit([1000] * 12, [1000] * 7)
elif seed == 5:
    emit([500] * 12, [500] * 13)
else:
    n = rng.randint(1, 1000)
    step = rng.choice([1, 10, 100, 250])
    emit([rng.randint(0, 1000 // step) * step for _ in range(12)],
         [rng.randint(0, 1000 // step) * step for _ in range(n)])
