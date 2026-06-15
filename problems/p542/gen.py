"""Tests for p542 (Gena vs Petya): 1 <= n <= 2e5, 1 <= ai <= 1e18.

Seed 1 is a single pile. Seeds 2 and 3 are maximal n = 2e5: seed 2 uses
near-1e18 random values, seed 3 makes every pile equal to 1e18 so the
candidate range of x is maximal (count up to ~1e18). Later seeds cover
all-equal piles, deliberately small minima (many candidate x), and random
mixes that exercise the carry handling in the digit DP.
"""

import random
import sys

MAXN = 2 * 10 ** 5
MAXV = 10 ** 18

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(a):
    sys.stdout.write(f"{len(a)}\n")
    sys.stdout.write(" ".join(map(str, a)) + "\n")


if seed == 1:
    emit([rng.randint(1, MAXV)])
    sys.exit()

if seed == 2:
    a = [rng.randint(MAXV - 10 ** 12, MAXV) for _ in range(MAXN)]
    emit(a)
    sys.exit()

if seed == 3:
    # All piles maximal and equal: minimum is 1e18, so x ranges over a huge
    # interval; an even count of equal piles makes many x give XOR 0.
    a = [MAXV] * MAXN
    emit(a)
    sys.exit()

if seed == 4:
    # All equal, odd count -> structure differs from even case.
    n = MAXN - 1
    val = rng.randint(1, MAXV)
    emit([val] * n)
    sys.exit()

if seed == 5:
    # Small minimum to force many candidate x but full size.
    a = [rng.randint(1, 4) for _ in range(MAXN)]
    # raise some to keep values varied but keep min small
    for _ in range(MAXN // 2):
        a[rng.randrange(MAXN)] = rng.randint(1, MAXV)
    a[rng.randrange(MAXN)] = 1
    emit(a)
    sys.exit()

# Random mid-size tests with varied magnitudes.
n = rng.randint(1, MAXN)
mode = seed % 3
if mode == 0:
    a = [rng.randint(1, 10 ** 6) for _ in range(n)]
elif mode == 1:
    a = [rng.randint(1, MAXV) for _ in range(n)]
else:
    base = rng.randint(1, MAXV)
    a = [base + rng.randint(-min(base - 1, 1000), 1000) for _ in range(n)]
emit(a)
