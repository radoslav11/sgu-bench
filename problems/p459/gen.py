"""Tests for p459 (Choreographer Problem): 1 <= k <= n <= 20.

The whole input is the pair (n, k). Seed 1 is the minimal case, seed 2
the maximal one (n=20, k=20, about 2^20 stage sets), seed 3 a second
heavy case with k < n so both recursion branches stay non-trivial.
Later seeds fix the remaining interesting shapes (k=1, k=2, k=n-1) and
random pairs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1)
elif seed == 2:
    print(20, 20)
elif seed == 3:
    print(20, 10)
elif seed == 4:
    print(20, 1)
elif seed == 5:
    print(20, 19)
elif seed == 6:
    print(2, 1)
elif seed == 7:
    print(19, 2)
else:
    n = rng.randint(1, 20)
    print(n, rng.randint(1, n))
