"""Tests for p483 (Jealous Cucumber).

Input: N H with 1 <= N <= 1e9 and 0 <= H <= 1e9. The output must contain
at least one killed pumpkin, which requires (H+1)^2 <= N, so H is at most
about sqrt(N) - 1. The number of killed pumpkins is O(sqrt(N)).

Seed 1 is the minimal N=1 H=0. Seed 2 maximizes the output length:
N=1e9 with H=0 (the bottom row, ~63245 numbers). Seed 3 maximizes H at
N=1e9 (deepest endangered row that is still populated). Later seeds vary
N and H including the boundary where exactly one pumpkin is killed.
"""

import math
import random
import sys

NMAX = 10 ** 9

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, h):
    sys.stdout.write(f"{n} {h}\n")


def max_h_for(n):
    # Largest H with (H+1)^2 <= N -> output non-empty.
    return int(math.isqrt(n)) - 1


if seed == 1:
    emit(1, 0)
    sys.exit()

if seed == 2:
    # Maximal output length: bottom row of the largest hutch.
    emit(NMAX, 0)
    sys.exit()

if seed == 3:
    # Maximal H still producing output at the largest N.
    emit(NMAX, max_h_for(NMAX))
    sys.exit()

if seed == 4:
    # Large N, mid-depth row (still long output).
    emit(NMAX, max_h_for(NMAX) // 2)
    sys.exit()

if seed == 5:
    # Boundary: N is a perfect square so the center pumpkin (H+1)^2 = N is the
    # last and the row barely fits, yielding a long but tight output.
    h = 31621  # (31622)^2 = 999950884 <= 1e9
    emit((h + 1) * (h + 1), h)
    sys.exit()

if seed == 6:
    # Single-pumpkin output: N equals the center index for a deep row, so no
    # neighbour fits and exactly one number is printed.
    h = 20000
    emit((h + 1) * (h + 1), h)
    sys.exit()

# Random tests: pick N, then a valid H in [0, max_h_for(N)].
n = rng.randint(1, NMAX)
hi = max(0, max_h_for(n))
h = rng.randint(0, hi)
emit(n, h)
