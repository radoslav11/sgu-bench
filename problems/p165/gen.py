"""Tests for p165 (Basketball): N <= 6000 heights in [1.95, 2.05] m,
integer micrometers, average exactly 2.00 m.

Seed 1 is N=1. Seeds 2-3 are maximal N=6000 (extreme +-0.05 deviations vs
a smooth random spread). Later seeds mix sizes and distributions: heavy
ties, all-equal heights, few distinct values, skewed clusters.
"""

import random
import sys

LO, HI = -50000, 50000  # deviation from 2.0 m, in micrometers


def balance(rng, devs):
    """Nudge random entries within [LO, HI] until the total is zero."""
    total = sum(devs)
    n = len(devs)
    while total != 0:
        j = rng.randrange(n)
        if total > 0:
            take = min(devs[j] - LO, total)
            devs[j] -= take
            total -= take
        else:
            take = min(HI - devs[j], -total)
            devs[j] += take
            total += take
    return devs


def emit(devs):
    print(len(devs))
    print(" ".join(f"2.{d:06d}" if d >= 0 else f"1.{1000000 + d:06d}"
                   for d in devs))


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit([0])
elif seed == 2:
    # maximal N, almost everyone at the +-0.05 extremes
    n = 6000
    devs = [rng.choice([LO, HI]) for _ in range(n)]
    emit(balance(rng, devs))
elif seed == 3:
    # maximal N, smooth uniform spread of deviations
    n = 6000
    devs = [rng.randint(LO, HI) for _ in range(n)]
    emit(balance(rng, devs))
elif seed == 4:
    # all players exactly 2.00 m
    emit([0] * 6000)
elif seed == 5:
    # only a few distinct heights, lots of ties
    n = 6000
    vals = [rng.randint(LO, HI) for _ in range(4)]
    devs = [rng.choice(vals) for _ in range(n)]
    emit(balance(rng, devs))
elif seed == 6:
    # one tall cluster balanced by many slightly short players
    n = 6000
    k = n // 10
    devs = [HI] * k + [-(HI * k) // (n - k)] * (n - k)
    emit(balance(rng, devs))
else:
    n = rng.randint(2, 6000)
    devs = [rng.randint(LO, HI) for _ in range(n)]
    emit(balance(rng, devs))
