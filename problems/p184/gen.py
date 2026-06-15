"""Tests for p184 (Patties): max patties = min(P/K, M/R, C/V).

All six numbers are in [1, 10000]. Seed 1 is all-ones, seed 2 the
maximal stock with minimal per-patty needs (largest answer), seed 3 the
opposite extreme (answer 0). Other seeds are random, including exact
divisibility and ties between the three ratios.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1, 1, 1, 1, 1)
elif seed == 2:
    print(10000, 10000, 10000, 1, 1, 1)
elif seed == 3:
    print(1, 1, 1, 10000, 10000, 10000)
elif seed == 4:
    # exact divisibility ties on all three ingredients
    print(9999, 9999, 9999, 3, 3, 3)
else:
    print(rng.randint(1, 10000), rng.randint(1, 10000), rng.randint(1, 10000),
          rng.randint(1, 10000), rng.randint(1, 10000), rng.randint(1, 10000))
