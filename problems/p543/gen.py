"""Tests for p543 (Cafe): 1 <= n <= 2000, 3 <= r <= 2000, 2 <= ai <= 2000.

Seed 1 is the minimal case (one group of two, smallest table). Seeds 2 and
3 are maximal n = 2000 with every group of the maximum size 2000: seed 2
uses an odd table size r = 2001-capped (r = 1999) to exercise the odd-r
casework, seed 3 uses an even r. Later seeds vary n, r and the group-size
distribution, including all-odd groups, all-even groups, r values around 6
where the 6-seat absorption kicks in, and mixed random sizes.
"""

import random
import sys

MAXN = 2000
MAXR = 2000
MAXA = 2000

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(r, a):
    sys.stdout.write(f"{len(a)} {r}\n")
    sys.stdout.write(" ".join(map(str, a)) + "\n")


if seed == 1:
    emit(3, [2])
    sys.exit()

if seed == 2:
    # Max n, max group sizes, odd table size.
    emit(1999, [MAXA] * MAXN)
    sys.exit()

if seed == 3:
    # Max n, max group sizes, even table size.
    emit(MAXR, [MAXA] * MAXN)
    sys.exit()

if seed == 4:
    # All odd groups (each forces at least one 3-chunk), odd table size.
    a = [rng.randrange(3, MAXA + 1, 2) for _ in range(MAXN)]
    emit(rng.randrange(3, MAXR, 2), a)
    sys.exit()

if seed == 5:
    # All even groups, small even table size emphasizing 2-chunk packing.
    a = [rng.randrange(2, MAXA + 1, 2) for _ in range(MAXN)]
    emit(rng.choice([4, 6, 8]), a)
    sys.exit()

if seed == 6:
    # r around 6 where two 3-chunks absorb into one table.
    a = [rng.randint(2, MAXA) for _ in range(MAXN)]
    emit(rng.choice([5, 6, 7]), a)
    sys.exit()

# Random mid-size tests.
n = rng.randint(1, MAXN)
r = rng.randint(3, MAXR)
a = [rng.randint(2, MAXA) for _ in range(n)]
emit(r, a)
