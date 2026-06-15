"""Tests for p154 (Factorial): 0 <= Q <= 10^8, find min N with Q zeros.

Seed 1 is Q=0, seed 2 the maximal Q=10^8, seed 3 a near-maximal Q that
falls in a gap of Z(N) (no solution). Seed 4 hits Q produced exactly at
a high power of five (the jump skips Q-1). The rest are random across
magnitudes, half of them shifted to land on unreachable values.
"""

import random
import sys


def zeros(x):
    total = 0
    power = 5
    while power <= x:
        total += x // power
        power *= 5
    return total


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    q = 0
elif seed == 2:
    q = 10**8
elif seed == 3:
    # walk down from 10^8 - 1 to the first value skipped by Z(N)
    q = 10**8 - 1
    while True:
        # find min n with zeros(n) >= q and check exactness
        lo, hi, best = 1, 2 * 10**12, -1
        while lo <= hi:
            mid = (lo + hi) // 2
            if zeros(mid) >= q:
                best, hi = mid, mid - 1
            else:
                lo = mid + 1
        if zeros(best) != q:
            break
        q -= 1
elif seed == 4:
    q = zeros(5**11)  # answer is an exact power of five
else:
    q = rng.randint(0, 10**8)
    if seed % 2 == 0:
        q = zeros(rng.randint(1, 4 * 10**8))  # guaranteed reachable

print(q)
