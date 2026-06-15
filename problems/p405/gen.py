"""Tests for p405 (Totalizator): n, m <= 100, goals 0..10.

Seed 1 is minimal (1 participant, 1 match). Seeds 2-3 are maximal
100x100 blocks: seed 2 uniform random goals, seed 3 a low-score
distribution (0..2) so ties/winner/difference bonuses fire constantly.
Later seeds mix random sizes and all-equal predictions.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1)
    print(0, 0)
    print(0, 0)
    sys.exit()

if seed == 2:
    n, m, lo, hi = 100, 100, 0, 10
elif seed == 3:
    n, m, lo, hi = 100, 100, 0, 2
else:
    n, m = rng.randint(1, 100), rng.randint(1, 100)
    lo, hi = 0, rng.choice([1, 3, 10])

print(n, m)
for _ in range(m):
    print(rng.randint(lo, hi), rng.randint(lo, hi))
    for _ in range(n):
        print(rng.randint(lo, hi), rng.randint(lo, hi))
