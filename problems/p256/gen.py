"""Tests for p256 (Balloons): 1 <= M <= 100, 1 <= N <= 10,
0 <= Ai <= 10, 1 <= Bi <= 4. Always at least one Ai > 0 so the answer
is well defined.

Seed 1 is minimal (M=1, N=1). Seed 2 is the slowest shape: M=100,
N=10, all Ai=1, Bi=4 (long schedule, big state space). Seed 3 is
another full-size test with mixed values including useless Ai=0
volunteers. Later seeds are random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(m, ab):
    print(m, len(ab))
    for a, b in ab:
        print(a, b)


if seed == 1:
    emit(1, [(1, 1)])
elif seed == 2:
    emit(100, [(1, 4)] * 10)
elif seed == 3:
    ab = [(rng.randint(0, 10), rng.randint(1, 4)) for _ in range(9)]
    ab.append((1, 4))
    rng.shuffle(ab)
    emit(100, ab)
elif seed == 4:
    # One strong volunteer among many zero-output ones.
    ab = [(0, rng.randint(1, 4)) for _ in range(9)] + [(10, 4)]
    rng.shuffle(ab)
    emit(100, ab)
else:
    m = rng.randint(1, 100)
    n = rng.randint(1, 10)
    ab = [(rng.randint(0, 10), rng.randint(1, 4)) for _ in range(n)]
    if all(a == 0 for a, _ in ab):
        ab[rng.randrange(n)] = (rng.randint(1, 10), rng.randint(1, 4))
    emit(m, ab)
