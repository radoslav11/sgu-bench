"""Tests for p410 (Galaxy in danger): N <= 100000 planets, counts are
natural numbers up to 1e9.

Seed 1 is minimal (one planet, one mistkafer). Seeds 2-3 are maximal
N=100000: seed 2 uniform values in [1, 1e9], seed 3 one planet at 1e9
and the rest tiny so every planet needs ~30 doublings. Seeds 4-6 keep
the answer <= 1000 so the schedule must be produced: seed 4 values in
[M/2, M] (one timed clone each), seed 5 tiny values vs a big max
(pre-doubling chains), seed 6 all planets equal (pure flying). The rest
are random mid cases.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1)
    print(1)
elif seed == 2:
    n = 100000
    print(n)
    print(*[rng.randint(1, 10**9) for _ in range(n)])
elif seed == 3:
    n = 100000
    vals = [rng.randint(1, 3) for _ in range(n - 1)] + [10**9]
    rng.shuffle(vals)
    print(n)
    print(*vals)
elif seed == 4:
    n, m = 300, 600
    vals = [m] + [rng.randint(m // 2, m) for _ in range(n - 1)]
    rng.shuffle(vals)
    print(n)
    print(*vals)
elif seed == 5:
    n, m = 4, 900
    vals = [m, 1, 2, 7]
    print(n)
    print(*vals)
elif seed == 6:
    n = 1000
    print(n)
    print(*([777] * n))
else:
    n = rng.randint(2, 5000)
    hi = rng.choice([10, 1000, 10**6, 10**9])
    print(n)
    print(*[rng.randint(1, hi) for _ in range(n)])
