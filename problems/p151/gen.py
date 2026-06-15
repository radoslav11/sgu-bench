"""Tests for p151 (construct triangle from c=|AB|, b=|AC|, median m=|AM|).

All three values are in (0, 1000] with at most 2 fractional digits.
Feasible cases are produced by sampling b, c and the angle at A, then
computing the median length, keeping a safety margin from degeneracy so
that rounding to 2 decimals cannot flip the answer. Infeasible cases use
either m far above sqrt((b^2+c^2)/2) (negative a^2) or m far below the
value needed for a < b + c.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def fmt(x):
    return f"{x:.2f}"


def feasible(rng, lo, hi):
    while True:
        b = rng.uniform(lo, hi)
        c = rng.uniform(lo, hi)
        theta = rng.uniform(0.15, math.pi - 0.15)
        a2 = b * b + c * c - 2 * b * c * math.cos(theta)
        m2 = (2 * b * b + 2 * c * c - a2) / 4
        m = math.sqrt(m2)
        if 0.05 <= m <= 999.99:
            return b, c, m


if seed == 1:
    print("0.02 0.02 0.01")  # near-minimal values, feasible triangle
elif seed == 2:
    print("1000.00 1000.00 500.00")  # maximal sides, feasible
elif seed == 3:
    print("1000.00 1000.00 999.00")  # maximal sides, thin triangle
elif seed == 4:
    print("100.00 100.00 900.00")  # m too long: a^2 < 0
elif seed == 5:
    print("600.00 100.00 100.00")  # m too short: forces a > b + c
elif seed == 6:
    print("5 5 3")  # the statement sample
else:
    kind = rng.choice(["feasible", "neg_a2", "too_long"])
    if kind == "feasible":
        b, c, m = feasible(rng, 0.5, 999.0)
        print(fmt(c), fmt(b), fmt(m))
    elif kind == "neg_a2":
        b = rng.uniform(1, 500)
        c = rng.uniform(1, 500)
        cap = math.sqrt((b * b + c * c) / 2)
        m = min(1000.0, cap * rng.uniform(1.2, 1.8))
        print(fmt(c), fmt(b), fmt(m))
    else:
        # a > b + c: need m well below sqrt(2b^2+2c^2-(b+c)^2)/2
        while True:
            b = rng.uniform(200, 900)
            c = rng.uniform(1, b / 3)
            lim2 = 2 * b * b + 2 * c * c - (b + c) ** 2
            if lim2 > 100:
                m = math.sqrt(lim2) / 2 * rng.uniform(0.2, 0.7)
                if m >= 0.05:
                    break
        print(fmt(c), fmt(b), fmt(m))
