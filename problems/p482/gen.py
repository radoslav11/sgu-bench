"""Tests for p482 (Impudent Thief).

Input: n (1 <= n <= 50), then n heights h_i (1 <= h_i <= 100).
Seed 1 is the minimal single board. Seeds 2 and 3 are maximal n = 50:
seed 2 has wildly alternating heights (large perimeter, many ties to
break), seed 3 is a smooth ramp. Later seeds cover all-equal heights,
extreme alternation between 1 and 100, and random mid-size fences.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(h):
    sys.stdout.write(f"{len(h)}\n")
    sys.stdout.write(" ".join(map(str, h)) + "\n")


if seed == 1:
    emit([rng.randint(1, 100)])
    sys.exit()

if seed == 2:
    # Maximal n with large, jagged heights -> big perimeter, full DP range.
    emit([rng.randint(1, 100) for _ in range(50)])
    sys.exit()

if seed == 3:
    # Maximal n, monotone ramp then down (smooth, small interior perimeter).
    up = list(range(1, 26))
    down = list(range(25, 0, -1))
    emit((up + down)[:50])
    sys.exit()

if seed == 4:
    # All boards equal -> minimal interior perimeter, ties everywhere.
    emit([100] * 50)
    sys.exit()

if seed == 5:
    # Extreme alternation 1,100,1,100,... maximizes perimeter.
    emit([1 if i % 2 == 0 else 100 for i in range(50)])
    sys.exit()

if seed == 6:
    # All ones.
    emit([1] * 50)
    sys.exit()

# Random mid-size fences with varied height ranges.
n = rng.randint(1, 50)
hmax = rng.choice([2, 5, 10, 100])
emit([rng.randint(1, hmax) for _ in range(n)])
