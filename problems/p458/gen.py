"""Tests for p458 (The Monochrome Picture): n <= 1e5, 0 <= c_i <= 1e6.

Seed 1 is minimal (n=1), seed 2 is maximal with colors over the full
domain, seed 3 is maximal with a two-color comb (x, x+1, x, x+1, ...)
forcing about half the pixels out. Later seeds cover the consecutive
ramp 0,1,2,..., all-equal pictures, tiny color ranges full of +-1
conflicts, and random data.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

N = 10**5
V = 10**6

if seed == 1:
    print(1)
    print(0)
    sys.exit()

if seed == 2:
    vals = [rng.randint(0, V) for _ in range(N)]
elif seed == 3:
    vals = [500000 + (i & 1) for i in range(N)]
elif seed == 4:
    # increasing ramp wrapping the full color range
    vals = [i % (V + 1) for i in range(N)]
elif seed == 5:
    # all pixels the same color (already a valid picture)
    vals = [rng.randint(0, V)] * N
elif seed == 6:
    # tiny color range -> dense +-1 conflicts, value 0 boundary included
    vals = [rng.randint(0, 3) for _ in range(N)]
elif seed == 7:
    # boundary colors 0 and 10^6 mixed with their neighbours
    vals = [rng.choice([0, 1, V - 1, V]) for _ in range(N)]
else:
    n = rng.randint(1, N)
    span = rng.choice([1, 2, 10, 1000, V])
    base = rng.randint(0, V - span)
    vals = [base + rng.randint(0, span) for _ in range(n)]

print(len(vals))
print(" ".join(map(str, vals)))
