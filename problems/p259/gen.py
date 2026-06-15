"""Tests for p259 (Printed PR): N <= 100 leaflets, 1 <= Ti, Li <= 1000.

Seed 1 is minimal (N=1). Seed 2 is maximal with all values at 1000,
seed 3 maximal with random values. Seeds 4-5 are anti-greedy: all
delivery times equal (order decided by ties) and delivery inversely
correlated with print time. The rest are random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(ts, ls):
    print(len(ts))
    print(" ".join(map(str, ts)))
    print(" ".join(map(str, ls)))


if seed == 1:
    emit([1], [1])
elif seed == 2:
    emit([1000] * 100, [1000] * 100)
elif seed == 3:
    emit(
        [rng.randint(1, 1000) for _ in range(100)],
        [rng.randint(1, 1000) for _ in range(100)],
    )
elif seed == 4:
    l = rng.randint(1, 1000)
    emit([rng.randint(1, 1000) for _ in range(100)], [l] * 100)
elif seed == 5:
    ts = [rng.randint(1, 1000) for _ in range(100)]
    ls = [max(1, 1001 - t) for t in ts]
    emit(ts, ls)
else:
    n = rng.randint(1, 100)
    hi = rng.choice([3, 10, 1000])
    emit(
        [rng.randint(1, hi) for _ in range(n)],
        [rng.randint(1, hi) for _ in range(n)],
    )
