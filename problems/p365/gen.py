"""Tests for p365 (Ships of the Desert). Input is a single integer
S in 1..20, so the whole input space has 20 points; the maximal test is
simply S = 20.

Seed plan:
- seed 1: S = 1 (minimal).
- seed 2: S = 20 (maximal).
- seed 3: S = 19 (second largest, different parity).
- seeds 4+: distinct values sweeping the rest of the range.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

fixed = {1: 1, 2: 20, 3: 19, 4: 2, 5: 3, 6: 10}

if seed in fixed:
    s = fixed[seed]
else:
    s = rng.randint(4, 18)

print(s)
