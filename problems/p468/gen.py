"""Tests for p468 (A bit of classic): one integer N, 1 <= N <= 250.

Open knight's tours exist for N = 1 and N >= 5; N = 2, 3, 4 have none.
Seed 1 is N=1, seeds 2-3 the maximal boards 250 and 249, seeds 4-6 the
unsolvable sizes, the rest small and mid-size boards.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

fixed = {1: 1, 2: 250, 3: 249, 4: 2, 5: 3, 6: 4, 7: 5, 8: 6}
if seed in fixed:
    print(fixed[seed])
else:
    print(rng.randint(7, 248))
