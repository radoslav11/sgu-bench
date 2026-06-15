"""Tests for p338 (Casino): one integer X, 0 <= X <= 1000.

The whole input is a single number, so "size" never varies; the seeds
sweep the boundary values (0, 1000, 999, 1) plus random interior X.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(0)
elif seed == 2:
    print(1000)
elif seed == 3:
    print(999)
elif seed == 4:
    print(1)
elif seed == 5:
    print(2)
else:
    print(rng.randint(3, 998))
