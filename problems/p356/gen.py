"""Tests for p356 (Extrasensory Perception): one line "K N".

Constraints: 1 <= N <= 100, 0 <= K <= N. Seed 1 is the minimal case,
seeds 2-3 are maximal N=100 with extreme/middle K, then the special
K = N-1 (probability 0) and K = N corners, followed by random pairs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1)
elif seed == 2:
    print(0, 100)
elif seed == 3:
    print(50, 100)
elif seed == 4:
    print(99, 100)
elif seed == 5:
    print(100, 100)
elif seed == 6:
    print(0, 1)
elif seed == 7:
    print(1, 2)
else:
    n = rng.randint(1, 100)
    print(rng.randint(0, n), n)
