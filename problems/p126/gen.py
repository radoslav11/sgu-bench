"""Tests for p126 (Boxes): 0 <= A, B with 0 < A + B < 2^31.

Seed 1 is the minimal total (one ball), seed 2 the maximal odd total
2147483647 (solvable only when one box is empty), seed 3 a maximal
solvable split A = B near the limit. Later seeds mix solvable cases
built as (x*2^k, y*2^k) with x+y a power of two, and fully random
pairs that are usually impossible.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 0)
elif seed == 2:
    a = rng.randint(1, 2147483646)
    print(a, 2147483647 - a)
elif seed == 3:
    print(1073741823, 1073741823)
elif seed == 4:
    print(0, 2147483647)
elif seed % 2 == 1:
    # solvable: total = (x + y) * 2^k with x + y a power of two
    p = rng.randint(1, 20)
    x = rng.randint(1, (1 << p) - 1)
    y = (1 << p) - x
    k = rng.randint(0, 30 - p - 1)
    print(x << k, y << k)
else:
    total = rng.randint(2, 2147483646)
    a = rng.randint(0, total)
    print(a, total - a)
