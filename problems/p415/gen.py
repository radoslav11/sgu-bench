"""Tests for p415 (Necessary Coins).

n <= 200 distinct coins, 1 <= a_i <= x <= 10^4, and some subset must sum
exactly to x. Each test plants a random subset of distinct values summing
to x, then pads with distinct filler values. Seed 1 is minimal, seeds 2-3
are maximal (n = 200, x = 10^4) with different value distributions, seed 6
makes every coin necessary (total sum equals x).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def planted_subset(x, lo, hi, rng):
    # Distinct values from [lo, hi] summing exactly to x (retry loop).
    while True:
        chosen = set()
        total = 0
        for _ in range(1000):
            v = rng.randint(lo, min(hi, x - total))
            if v not in chosen and total + v <= x:
                chosen.add(v)
                total += v
            if total == x:
                return chosen
            r = x - total
            if lo <= r <= hi and r not in chosen:
                chosen.add(r)
                return chosen


def emit(x, coins, rng):
    coins = list(coins)
    rng.shuffle(coins)
    print(len(coins), x)
    print(" ".join(map(str, coins)))


if seed == 1:
    print(1, 1)
    print(1)
    sys.exit()

if seed in (2, 3):
    n, x = 200, 10000
    if seed == 2:
        base = planted_subset(x, 1, x, rng)
    else:
        # Small dense values: huge number of valid subsets.
        base = planted_subset(x, 1, 300, rng)
    pool_hi = 300 if seed == 3 else x
    while len(base) < n:
        v = rng.randint(1, pool_hi)
        if v not in base:
            base.add(v)
    emit(x, base, rng)
    sys.exit()

if seed == 6:
    # Every coin necessary: distinct values whose total sum is exactly x.
    x = 10000
    base = planted_subset(x, 1, 145, rng)
    emit(x, base, rng)
    sys.exit()

x = rng.randint(1, 10000)
base = planted_subset(x, 1, x, rng)
n = min(rng.randint(1, 200), x)
n = max(n, len(base))
while len(base) < n:
    v = rng.randint(1, x)
    if v not in base:
        base.add(v)
emit(x, base, rng)
