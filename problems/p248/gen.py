"""Tests for p248 (Integer Linear Programming): unbounded coin change with
N <= 3 coins, c[i] <= 10^6, V <= 10^6, minimize coin count.

Seed 1 is the minimal case, seed 2 a max-V test with small coprime coins,
seed 3 a max-V infeasible test (all coins even, V odd). Later seeds mix
random coins, huge coins above V, and duplicated coin values.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXV = 10**6

if seed == 1:
    print(1)
    print(1)
    print(1)
elif seed == 2:
    print(3)
    print(2, 3, 999983)
    print(MAXV)
elif seed == 3:
    # Infeasible at full size: every coin even, target odd.
    print(3)
    print(2, 500000, 999998)
    print(MAXV - 1)
elif seed == 4:
    # Single coin larger than V.
    print(1)
    print(MAXV)
    print(MAXV - 1)
elif seed == 5:
    # Two equal coins.
    print(2)
    print(7, 7)
    print(MAXV - MAXV % 7)
else:
    n = rng.randint(1, 3)
    coins = [rng.choice([rng.randint(1, 20), rng.randint(1, MAXV)]) for _ in range(n)]
    v = rng.randint(1, MAXV)
    print(n)
    print(*coins)
    print(v)
