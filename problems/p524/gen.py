"""Tests for p524 (Buoys): n buoys at distinct integer coords in [-10000,10000].

Seed 1 is the minimal n=2 case. Seeds 2-3 are maximal n=400 with the full
coordinate span (seed 2 nearly-evenly spaced, seed 3 a wildly irregular
spread). Later seeds mix random sizes, near-arithmetic inputs, clustered
points, and extreme-slope inputs.
"""

import random
import sys

LO, HI = -10000, 10000


def distinct_sorted(rng, n, lo=LO, hi=HI):
    span = hi - lo + 1
    if n > span:
        n = span
    vals = rng.sample(range(lo, hi + 1), n)
    vals.sort()
    return vals


def emit(n, xs):
    print(n)
    print(" ".join(str(v) for v in xs))


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit(2, [-10000, 10000])
    sys.exit()

if seed == 2:
    # Maximal n with near-uniform spacing across the full span.
    n = 400
    step = (HI - LO) / (n - 1)
    xs = []
    used = set()
    cur = LO
    for i in range(n):
        v = int(round(LO + step * i)) + rng.randint(-2, 2)
        v = max(LO, min(HI, v))
        while v in used or (xs and v <= xs[-1]):
            v += 1
        used.add(v)
        xs.append(v)
    # ensure last within bounds and strictly increasing already
    emit(n, xs)
    sys.exit()

if seed == 3:
    # Maximal n with a highly irregular spread (clusters + gaps).
    n = 400
    xs = distinct_sorted(rng, n)
    # Skew distribution: many near the ends, few in the middle.
    pool = []
    for _ in range(2000):
        r = rng.random()
        if r < 0.4:
            pool.append(rng.randint(LO, LO + 1000))
        elif r < 0.8:
            pool.append(rng.randint(HI - 1000, HI))
        else:
            pool.append(rng.randint(LO, HI))
    uniq = sorted(set(pool))
    if len(uniq) >= n:
        xs = sorted(rng.sample(uniq, n))
    emit(n, xs)
    sys.exit()

if seed == 4:
    # Already an arithmetic progression (answer should be ~0).
    n = rng.randint(2, 400)
    step = max(1, (HI - LO) // (n - 1))
    start = rng.randint(LO, LO + 100)
    xs = [start + step * i for i in range(n)]
    xs = [v for v in xs if v <= HI]
    n = len(xs)
    emit(n, xs)
    sys.exit()

if seed == 5:
    # Two tight clusters far apart.
    n = rng.randint(10, 400)
    half = n // 2
    left = distinct_sorted(rng, half, LO, LO + 5)
    right = distinct_sorted(rng, n - half, HI - 5, HI)
    xs = left + right
    emit(len(xs), xs)
    sys.exit()

# Random mid-size tests.
n = rng.randint(2, 400)
xs = distinct_sorted(rng, n)
emit(len(xs), xs)
