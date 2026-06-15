"""Tests for p112 (ab-ba): two integers 1 <= a, b <= 100.

The trusted reference prints an empty line when the result is 0, so
zero-result pairs (a == b, and {2, 4}) are never generated. Seed 1 is
the minimal (1, 2), seed 2 the maximal asymmetric pair (100, 99),
seed 3 the mirrored (99, 100) (negative answer), seed 4 the (1, 100)
extreme, seed 5 (100, 1), the rest random non-zero-result pairs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 2)
elif seed == 2:
    print(100, 99)
elif seed == 3:
    print(99, 100)
elif seed == 4:
    print(1, 100)
elif seed == 5:
    print(100, 1)
else:
    a = b = 1
    while a == b or {a, b} == {2, 4}:
        a, b = rng.randint(1, 100), rng.randint(1, 100)
    print(a, b)
