"""Tests for p371 (Subway): build a chain of 3..10-station circles plus
two-station radial lines, using exactly N stations and M segments.

The statement's N, M bounds were lost; tests stay within N <= 10^5 and
M <= 1.6 * N, where every structurally possible instance lives (the
number of circles is forced to K = M - N + 1 and 3K <= N + K - 1).
Seed 1 is the minimal "No solution", seed 2 maximal with all-triangle
circles, seed 3 maximal with all-decagon circles and many radials.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXN = 100000

if seed == 1:
    print(1, 0)
elif seed == 2:
    # n=100000, k=49999 circles -> nearly all triangles, almost no radials.
    print(100000, 100000 + 49999 - 1)
elif seed == 3:
    # n=100000, k=5883 (minimum feasible) -> all circles size 10, ~41000
    # radial lines.
    print(100000, 100000 + 5883 - 1)
elif seed == 4:
    # Large "No solution": too many circles to fit in n stations.
    print(100000, 160000)
elif seed == 5:
    # Tree-like edge count (k = 0) -> "No solution".
    print(4, 3)
elif seed == 6:
    # Smallest valid instance: a single triangle.
    print(3, 3)
elif seed == 7:
    # k just below the feasible band -> "No solution".
    print(100000, 100000 + 5881 - 1)
else:
    n = rng.randint(1, MAXN)
    lo = max(0, n - 2)
    m = rng.randint(lo, int(1.6 * n) + 2)
    print(n, m)
