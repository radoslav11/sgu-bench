"""Tests for p291 (Evolution): q,c <= 1000 grid, n <= 22204 cultures, t <= 1e9.

Input: "q c n t" then n lines with distinct start cells (x in 1..q,
y in 1..c). Seed 1 is the 1x1 grid; seed 2 is the full max (1000x1000,
n=22204 scattered, t=1e9); seed 3 is max-size with all sources packed
into one corner block so waves travel the whole grid. Other seeds probe
t=0, t=1, thin 1xC strips, and small-t partial expansion.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(q, c, t, starts):
    print(q, c, len(starts), t)
    for x, y in starts:
        print(x, y)


def scattered(q, c, n):
    cells = rng.sample(range(q * c), n)
    return [(z // c + 1, z % c + 1) for z in cells]


if seed == 1:
    emit(1, 1, 0, [(1, 1)])
elif seed == 2:
    emit(1000, 1000, 10**9, scattered(1000, 1000, 22204))
elif seed == 3:
    # all 22204 sources in a 150x149 corner block
    starts = [(x, y) for x in range(1, 151) for y in range(1, 150)]
    starts = starts[:22204]
    rng.shuffle(starts)
    emit(1000, 1000, 10**9, starts)
elif seed == 4:
    emit(1000, 1000, 0, scattered(1000, 1000, 22204))
elif seed == 5:
    emit(1000, 1000, 1, scattered(1000, 1000, 5000))
elif seed == 6:
    emit(1, 1000, rng.randint(0, 1000), scattered(1, 1000, 600))
elif seed == 7:
    # huge grid, few sources, small t: expansion must stop at distance t
    emit(1000, 1000, rng.randint(2, 40), scattered(1000, 1000, 8))
else:
    q, c = rng.randint(1, 60), rng.randint(1, 60)
    n = rng.randint(1, min(q * c, 500))
    emit(q, c, rng.choice([0, 1, 2, 3, 5, 17, 10**9]), scattered(q, c, n))
