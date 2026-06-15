"""Tests for p514 (Polygon): 3 <= n <= 10 side lengths, 1 <= a_i <= 100.

The input must admit a simple polygon, i.e. the largest side is strictly
less than the sum of the others. Seed 1 is the minimal n=3 triangle, seeds
2 and 3 are maximal n=10 inputs (all sides 100, and a high-value spread),
the rest mix random valid polygons with degenerate/near-degenerate sets.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(a):
    print(len(a))
    print(*a)


def fix_polygon(a):
    # Ensure the polygon inequality: max side < sum of the others. If it
    # fails, cap the largest side so a simple polygon exists.
    while True:
        total = sum(a)
        mx = max(a)
        if mx < total - mx:
            return a
        idx = a.index(mx)
        rest = total - mx
        a[idx] = max(1, rest - 1)
        if a[idx] >= mx:
            a[idx] = mx - 1 if mx > 1 else 1


if seed == 1:
    emit([3, 4, 5])
elif seed == 2:
    emit([100] * 10)
elif seed == 3:
    # Maximal n with a wide spread that still forms a polygon.
    a = [100, 100, 100, 100, 100, 99, 50, 25, 13, 7]
    emit(fix_polygon(a))
elif seed == 4:
    # Even total split into two equal opposite groups -> area 0.
    emit([10, 20, 30, 10, 20, 30])
elif seed == 5:
    # n = 3, near-degenerate thin triangle.
    emit([100, 100, 1])
else:
    n = rng.randint(3, 10)
    a = [rng.randint(1, 100) for _ in range(n)]
    emit(fix_polygon(a))
