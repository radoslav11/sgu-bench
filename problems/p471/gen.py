"""Tests for p471 (Funny Game): up to 10 lines, each one N with 1 <= N <= 50.

Seed 1 is the single minimal line, seed 2 ten copies of the maximum
N = 50, seed 3 the ten largest values. Seeds 4-7 sweep the ranges
1-10, 11-20, 21-30, 31-40 so every N appears in some test, covering
all residues mod 4; seed 8 is random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    values = [1]
elif seed == 2:
    values = [50] * 10
elif seed == 3:
    values = list(range(41, 51))
    rng.shuffle(values)
elif seed in (4, 5, 6, 7):
    lo = (seed - 4) * 10 + 1
    values = list(range(lo, lo + 10))
    rng.shuffle(values)
else:
    values = [rng.randint(1, 50) for _ in range(rng.randint(1, 10))]

print("\n".join(map(str, values)))
