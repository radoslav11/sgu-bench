"""Tests for p188 (Factory guard): soldiers on a 1000 m circular fence.

1 <= N <= 20, 1 <= T <= 50, distinct integer positions 0..999, integer
speeds in [-100, 100] excluding 0. Seed 1 is a single soldier, seed 2
the maximal case (N=20, T=50, extreme opposite speeds), seed 3 maximal
with adjacent positions and +-1 speeds. Other seeds randomize positions,
speeds and T, including same-direction-only fleets (all answers 0).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(t, ls, vs):
    print(len(ls), t)
    print(" ".join(map(str, ls)))
    print(" ".join(map(str, vs)))


def rand_speed(rng, lo=1, hi=100):
    s = rng.randint(lo, hi)
    return s if rng.random() < 0.5 else -s


if seed == 1:
    emit(1, [rng.randint(0, 999)], [rand_speed(rng)])
elif seed == 2:
    ls = rng.sample(range(1000), 20)
    vs = [100 if i % 2 == 0 else -100 for i in range(20)]
    emit(50, ls, vs)
elif seed == 3:
    ls = list(range(20))
    vs = [1 if i % 2 == 0 else -1 for i in range(20)]
    emit(50, ls, vs)
elif seed == 4:
    # everyone moves clockwise: nobody ever "meets"
    ls = rng.sample(range(1000), 20)
    vs = [rng.randint(1, 100) for _ in range(20)]
    emit(50, ls, vs)
elif seed == 5:
    # head-on neighbours at positions 0 and 999 (wraparound meeting)
    emit(50, [0, 999], [-100, 100])
else:
    n = rng.randint(1, 20)
    t = rng.randint(1, 50)
    ls = rng.sample(range(1000), n)
    vs = [rand_speed(rng) for _ in range(n)]
    emit(t, ls, vs)
