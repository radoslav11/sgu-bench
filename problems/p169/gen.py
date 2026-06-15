"""Tests for p169 (Numbers): single integer K, 1 <= K <= 1000000.

The answer only depends on K mod 6 (with K=1 special), so the seeds
cover K=1, both maximal values 1000000 and 999997 (different residues),
every residue class mod 6 at small K, and random K.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1)
elif seed == 2:
    print(1000000)
elif seed == 3:
    print(999997)  # max-range K with K % 6 == 1
elif seed <= 9:
    print(seed - 2)  # K = 2..7 covers all residues mod 6
else:
    print(rng.randint(2, 1000000))
