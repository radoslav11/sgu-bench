"""Tests for p160 (Magic Multiplying Machine): 1 <= N <= 10000,
2 <= M <= 1000, lever values in 1..M.

Seed 1 is the minimal N=1, M=2 with a=1 (best is the empty subset).
Seed 2 is fully maximal and random; seed 3 is maximal with prime
M=997. Seed 4 mixes only multiples-of-M and ones (score stays 1),
seed 5 uses only even values with M=1000 (odd residues unreachable),
seed 6 makes every value M-1 (sign flipping). The rest are random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    n, m = 1, 2
    a = [1]
elif seed == 2:
    n, m = 10000, 1000
    a = [rng.randint(1, m) for _ in range(n)]
elif seed == 3:
    n, m = 10000, 997
    a = [rng.randint(1, m) for _ in range(n)]
elif seed == 4:
    n, m = 10000, 1000
    a = [rng.choice([m, 1]) for _ in range(n)]
elif seed == 5:
    n, m = 10000, 1000
    a = [rng.randrange(2, m + 1, 2) for _ in range(n)]
elif seed == 6:
    n, m = 10000, 999
    a = [m - 1] * n
else:
    n = rng.randint(1, 3000)
    m = rng.randint(2, 1000)
    a = [rng.randint(1, m) for _ in range(n)]

print(n, m)
print(" ".join(map(str, a)))
