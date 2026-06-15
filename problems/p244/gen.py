"""Tests for p244 (Height, Bisector and Median): three integers in [1, 100].

Fixed seeds cover the degenerate equalities (h = d = m feasible, h = d < m
and h < d = m infeasible), orderings that force NO, and tight YES cases
where the bisector is barely longer than the height; the rest are random
triples.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

fixed = {
    1: (1, 1, 1),
    2: (100, 100, 100),
    3: (1, 2, 100),
    4: (98, 99, 100),
    5: (1, 1, 2),
    6: (1, 100, 100),
    7: (100, 1, 1),
    8: (3, 4, 5),
    9: (50, 50, 100),
}

if seed in fixed:
    print(*fixed[seed])
elif seed == 10:
    # random feasible-ordered triple h <= d <= m
    vals = sorted(rng.randint(1, 100) for _ in range(3))
    print(*vals)
else:
    print(*(rng.randint(1, 100) for _ in range(3)))
