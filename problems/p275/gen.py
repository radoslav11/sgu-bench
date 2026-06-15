"""Tests for p275 (To xor or not to xor): N <= 100, 0 <= Ai <= 10^18.

Seed 1 is minimal (N=1, value 0). Seed 2 is maximal with 100 uniform
random 60-bit values. Seed 3 is maximal with a highly dependent set:
values built as XORs of a small hidden basis, plus duplicates and
zeros, so greedy-by-value approaches break. Other seeds mix powers of
two, equal values, small numbers and anti-greedy patterns.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXV = 10 ** 18

if seed == 1:
    print(1)
    print(0)
elif seed == 2:
    print(100)
    print(*[rng.randint(0, MAXV) for _ in range(100)])
elif seed == 3:
    base = [rng.randint(0, MAXV) for _ in range(7)]
    vals = []
    while len(vals) < 100:
        x = 0
        for b in base:
            if rng.random() < 0.5:
                x ^= b
        if x <= MAXV:
            vals.append(x)
    print(100)
    print(*vals)
elif seed == 4:
    vals = [1 << b for b in range(60)] + [0] * 40
    rng.shuffle(vals)
    print(100)
    print(*vals)
elif seed == 5:
    v = rng.randint(0, MAXV)
    print(100)
    print(*([v] * 100))
elif seed == 6:
    # anti-greedy: one huge number whose complement is achievable in parts
    big = (1 << 59)
    rest = [rng.randint(0, (1 << 59) - 1) for _ in range(99)]
    print(100)
    print(big, *rest)
else:
    n = rng.randint(1, 100)
    vals = [rng.choice([
        rng.randint(0, 15),
        rng.randint(0, MAXV),
        0,
        MAXV,
    ]) for _ in range(n)]
    print(n)
    print(*vals)
