"""Tests for p438 (The Glorious Karlutka River): max-flow over time layers.

N <= 50 piles, M <= 50 tourists, jump D <= 1000, width 0 < W <= 1000.
Seed 1 is the minimal IMPOSSIBLE case (no piles, D < W). Seed 2 is a
50-pile capacity-1 chain that pipelines 50 tourists (long schedule).
Seed 3 keeps the graph fully connected but cuts it with a zero-capacity
band, so flow stays below M through every time layer. Later seeds are
random, mixing D >= W one-jump cases, zero capacities, duplicate pile
coordinates and boundary Y values.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, m, d, w, piles):
    print(n, m, d, w)
    for x, y, c in piles:
        print(x, y, c)


if seed == 1:
    emit(0, 1, 0, 1, [])
elif seed == 2:
    w, d = 1000, 20
    piles = [(500, 20 * i, 1) for i in range(1, 50)]
    piles.append((500, 999, 1))
    emit(50, 50, d, w, piles)
elif seed == 3:
    w, d = 1000, 300
    piles = []
    for i in range(20):
        piles.append((500, rng.randint(1, 299), rng.randint(500, 1000)))
    for i in range(20):
        piles.append((500, rng.randint(701, 999), rng.randint(500, 1000)))
    for i in range(10):
        piles.append((500, rng.randint(400, 600), 0))
    emit(50, 50, d, w, piles)
elif seed == 4:
    w = 1000
    piles = [(rng.randint(1, 999), rng.randint(1, 999), rng.randint(0, 3))
             for _ in range(50)]
    emit(50, 50, 150, w, piles)
elif seed == 5:
    emit(0, 50, 1000, 1000, [])
elif seed == 6:
    base = [(rng.randint(1, 999), rng.randint(1, 99), rng.randint(0, 1000))
            for _ in range(10)]
    piles = [rng.choice(base) for _ in range(50)]
    emit(50, 37, 60, 100, piles)
else:
    w = rng.randint(2, 1000)
    n = rng.randint(0, 50)
    m = rng.randint(1, 50)
    d = rng.choice([0, rng.randint(1, w), rng.randint(1, 1000)])
    piles = []
    for _ in range(n):
        y = rng.choice([1, w - 1, rng.randint(1, w - 1)])
        piles.append((rng.randint(1, 999), y, rng.randint(0, 1000)))
    emit(n, m, d, w, piles)
