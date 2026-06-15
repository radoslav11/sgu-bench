"""Tests for p270 (Thimbles): N <= 100 thimbles, M <= 1000 swaps.

Seed 1 is minimal (N=2, M=1). Seed 2 is maximal with uniformly random
swaps. Seed 3 is maximal with clustered structure: swaps confined to
small groups of positions so the reachable set stays limited, plus many
repeated identical swaps. Other seeds mix chains touching position 1,
swap sets that never involve position 1, and random sizes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, swaps):
    print(n, len(swaps))
    for a, b in swaps:
        print(a, b)


def rand_swap(lo, hi):
    a, b = rng.sample(range(lo, hi + 1), 2)
    return (min(a, b), max(a, b))


if seed == 1:
    emit(2, [(1, 2)])
elif seed == 2:
    emit(100, [rand_swap(1, 100) for _ in range(1000)])
elif seed == 3:
    groups = [(1, 10), (11, 30), (31, 60), (61, 100)]
    swaps = []
    base = [rand_swap(*rng.choice(groups)) for _ in range(40)]
    while len(swaps) < 1000:
        swaps.append(rng.choice(base))
    emit(100, swaps)
elif seed == 4:
    # chain 1-2, 2-3, ..., reachable set grows along the chain
    n = 100
    swaps = [(i, i + 1) for i in range(1, n)]
    while len(swaps) < 1000:
        swaps.append(rand_swap(1, n))
    rng.shuffle(swaps)
    emit(n, swaps)
elif seed == 5:
    # position 1 never touched: the ball cannot move at all
    n = 100
    emit(n, [rand_swap(2, n) for _ in range(1000)])
else:
    n = rng.randint(2, 100)
    m = rng.randint(1, 1000)
    emit(n, [rand_swap(1, n) for _ in range(m)])
