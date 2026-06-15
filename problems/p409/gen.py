"""Tests for p409 (Berland Flag): one line "N K", 1 <= N <= 20,
0 <= K <= N^2.

Seed 1 is minimal (1 0), seed 2 maximal N with mid K (the 400x400 grid),
seed 3 maximal N with K = N^2 (all blue). Seeds 4-6 pin K = 0, K = 1 and
K = N corner shapes at N = 20; the rest are random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 0)
elif seed == 2:
    print(20, 200)
elif seed == 3:
    print(20, 400)
elif seed == 4:
    print(20, 0)
elif seed == 5:
    print(20, 1)
elif seed == 6:
    print(20, 20)
else:
    n = rng.randint(1, 20)
    print(n, rng.randint(0, n * n))
