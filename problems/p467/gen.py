"""Tests for p467 (Chessmaster).

Input: up to 100 lines, each a percentage P with 0 <= P <= 100.

Seed 1 is a single value, seed 2 has 100 values sweeping 0..99, seed 3
has 100 values hammering the answer boundaries (0/78/79/95/96/99/100),
the rest are random batches.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(rng.choice([0, 100]))
elif seed == 2:
    for p in range(100):
        print(p)
elif seed == 3:
    edges = [0, 1, 77, 78, 79, 94, 95, 96, 98, 99, 100]
    for _ in range(100):
        print(rng.choice(edges))
else:
    for _ in range(rng.randint(1, 100)):
        print(rng.randint(0, 100))
