"""Tests for p358 (Median of Medians). Input is always 3x3 integers in
[-1000, 1000], so every test is maximal-size by construction.

Seed plan:
- seed 1: all values at the lower bound -1000.
- seed 2: all values at the upper bound 1000.
- seed 3: mixed extremes -1000/0/1000.
- seed 4: all nine values equal (ties everywhere).
- seed 5: duplicates inside each row.
- seeds 6+: uniformly random values.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    rows = [[-1000, -1000, -1000]] * 3
elif seed == 2:
    rows = [[1000, 1000, 1000]] * 3
elif seed == 3:
    rows = [[-1000, 0, 1000], [1000, -1000, 0], [0, 1000, -1000]]
elif seed == 4:
    rows = [[7, 7, 7]] * 3
elif seed == 5:
    rows = [[5, 5, -3], [-3, 5, 5], [5, -3, 5]]
else:
    rows = [[rng.randint(-1000, 1000) for _ in range(3)] for _ in range(3)]

for row in rows:
    print(*row)
