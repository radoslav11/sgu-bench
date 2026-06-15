"""Tests for p362 (Robot-Annihilator). Input is N M (1..10) plus the
starting cell, so every test is tiny and the maximal grid is 10x10.

Seed plan:
- seed 1: 1x1 grid (the robot explodes immediately, empty move list).
- seed 2: 10x10 starting at corner (1,1) — longest spiral.
- seed 3: 10x10 starting at center (5,5) — different walk structure.
- seed 4: 1x10 row, start in the middle.
- seed 5: 10x1 column, start at the bottom.
- seed 6: 10x10 starting at corner (10,10).
- seeds 7+: random grid sizes and starts.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

fixed = {
    1: (1, 1, 1, 1),
    2: (10, 10, 1, 1),
    3: (10, 10, 5, 5),
    4: (1, 10, 1, 6),
    5: (10, 1, 10, 1),
    6: (10, 10, 10, 10),
}

if seed in fixed:
    n, m, si, sj = fixed[seed]
else:
    n, m = rng.randint(1, 10), rng.randint(1, 10)
    si, sj = rng.randint(1, n), rng.randint(1, m)

print(n, m)
print(si, sj)
