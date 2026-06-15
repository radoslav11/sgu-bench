"""Tests for p141 (Jumping Joe): 0 < x1,x2 < 40000, |P| < 40000,
0 <= K < 2e9.

Seed 1 is minimal (everything at the small end). Seeds 2-3 use the
maximal K: seed 2 a random reachable target, seed 3 equal odd steps
where the leftover-jump parity cannot be fixed (NO). Seeds 4-6 cover
gcd-infeasible targets and K=0 edge cases, the rest are random with a
bias toward common-divisor step pairs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    x1, x2, p, k = 1, 1, 0, 0
elif seed == 2:
    x1, x2, p, k = 39999, 39998, -39999, 1999999999
elif seed == 3:
    # gcd 39999, S_min = 1, leftover odd, dx+dy = 2 even -> NO
    x1, x2, p, k = 39999, 39999, 39999, 1999999998
elif seed == 4:
    x1, x2, p, k = 39998, 39996, 39997, 1999999999  # gcd 2, odd P -> NO
elif seed == 5:
    x1, x2, p, k = 17, 29, 0, 0  # zero jumps, already there -> YES
elif seed == 6:
    x1, x2, p, k = 17, 29, 5, 0  # zero jumps, wrong point -> NO
else:
    if seed % 2 == 0:
        d = rng.choice([2, 3, 5, 7, 12])
        x1 = d * rng.randint(1, 39999 // d)
        x2 = d * rng.randint(1, 39999 // d)
    else:
        x1 = rng.randint(1, 39999)
        x2 = rng.randint(1, 39999)
    p = rng.randint(-39999, 39999)
    k = rng.choice(
        [
            rng.randint(0, 10),
            rng.randint(0, 1000),
            rng.randint(1999999990, 1999999999),
        ]
    )

print(x1, x2, p, k)
