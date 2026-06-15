"""Tests for p178 (Golden chain): one integer N, 1 <= N <= 10^16.

The answer changes at thresholds N = (k+1)*2^(k+1) - 1, so besides the
two extremes (seed 1: N = 1, seed 2: N = 10^16) the seeds probe values
exactly at, just below and just above several thresholds, plus the
sample and random values across the whole range.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXN = 10**16

thresholds = []
k = 0
while True:
    t = (k + 1) * (1 << (k + 1)) - 1
    if t > MAXN:
        break
    thresholds.append(t)
    k += 1

if seed == 1:
    print(1)
elif seed == 2:
    print(MAXN)
elif seed == 3:
    print(MAXN - 1)
elif seed == 4:
    print(9)
elif seed == 5:
    print(thresholds[rng.randrange(len(thresholds))])
elif seed == 6:
    print(thresholds[rng.randrange(1, len(thresholds))] + 1)
elif seed == 7:
    print(max(1, thresholds[rng.randrange(len(thresholds))] - 1))
elif seed == 8:
    print(thresholds[-1])
elif seed == 9:
    print(thresholds[-1] + 1)
else:
    print(rng.randint(1, MAXN))
