"""Tests for p361 (National Flag). Input is just N M with 3 <= N, M <= 200;
the answer's blue count depends on N, M mod 3, so seeds sweep residue
combinations and extreme aspect ratios.

Seed plan:
- seed 1: 3 3 (minimal).
- seed 2: 200 200 (maximal).
- seed 3: 200 199 (maximal, different column residue).
- seed 4: 3 200 (flat strip).
- seed 5: 200 3 (tall strip).
- seed 6: 199 197 (both residues nonzero).
- seeds 7+: random N, M.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

fixed = {
    1: (3, 3),
    2: (200, 200),
    3: (200, 199),
    4: (3, 200),
    5: (200, 3),
    6: (199, 197),
}

if seed in fixed:
    n, m = fixed[seed]
else:
    n, m = rng.randint(3, 200), rng.randint(3, 200)

print(n, m)
