"""Tests for p183 (Painting the balls): min dye so that every window of
M successive balls holds at least two black ones.

2 <= N <= 10000, 2 <= M <= min(N, 100), 1 <= Ci <= 10000. Seed 1 is the
minimal N=M=2 case, seed 2 the maximal N=10000, M=100 with uniform random
costs, seed 3 the same size with an anti-greedy sawtooth cost pattern.
Later seeds cover M=2 (all balls painted), M=N windows and random sizes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAX_N = 10000
MAX_C = 10000


def emit(m, costs):
    print(len(costs), m)
    print(" ".join(map(str, costs)))


if seed == 1:
    emit(2, [rng.randint(1, MAX_C) for _ in range(2)])
elif seed == 2:
    emit(100, [rng.randint(1, MAX_C) for _ in range(MAX_N)])
elif seed == 3:
    # sawtooth: cheap balls right before expensive stretches (anti-greedy)
    costs = []
    for i in range(MAX_N):
        block = (i // 50) % 2
        costs.append(rng.randint(1, 10) if block == 0 else rng.randint(9000, MAX_C))
    emit(100, costs)
elif seed == 4:
    # M=2 forces painting every ball
    emit(2, [rng.randint(1, MAX_C) for _ in range(MAX_N)])
elif seed == 5:
    # window equal to whole line of 100 balls
    emit(100, [rng.randint(1, MAX_C) for _ in range(100)])
elif seed == 6:
    # all costs equal
    emit(100, [MAX_C] * MAX_N)
else:
    n = rng.randint(2, MAX_N)
    m = rng.randint(2, min(n, 100))
    hi = rng.choice([1, 10, MAX_C])
    emit(m, [rng.randint(1, hi) for _ in range(n)])
