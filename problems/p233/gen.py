"""Tests for p233 (The Greatest Angle): point C on circle maximizing ACB.

Up to N = 10000 test cases per file; circle center/radius are integers
with |X0|, |Y0|, R <= 10000, A and B are integer points strictly inside.
Points are sampled with |PA - O| <= R - 1 so the optimal angle is not
degenerately sensitive to 1e-6 output rounding. Seed 1 is a single tiny
case, seeds 2-3 are N = 10000 files (large radii vs small radii with
collinear / symmetric / A-at-center patterns).
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def inner_point(x0, y0, r):
    while True:
        x = rng.randint(x0 - r + 1, x0 + r - 1)
        y = rng.randint(y0 - r + 1, y0 + r - 1)
        if (x - x0) ** 2 + (y - y0) ** 2 <= (r - 1) ** 2:
            return x, y


def make_case(rmin, rmax):
    r = rng.randint(rmin, rmax)
    x0 = rng.randint(-10000, 10000)
    y0 = rng.randint(-10000, 10000)
    a = inner_point(x0, y0, r)
    while True:
        b = inner_point(x0, y0, r)
        if b != a:
            break
    style = rng.random()
    if style < 0.15:
        a = (x0, y0)  # A at the center
    elif style < 0.3 and r >= 3:
        d = rng.randint(1, r - 1)
        a, b = (x0 - d, y0), (x0 + d, y0)  # symmetric through center
    elif style < 0.45 and r >= 3:
        d1 = rng.randint(1, r - 1)
        d2 = rng.randint(1, r - 1)
        a, b = (x0 + d1, y0), (x0 + d2, y0)  # collinear with center
        if a == b:
            b = (x0 - d2, y0)
    if a == b:
        return make_case(rmin, rmax)
    return (x0, y0, r) + a + b


if seed == 1:
    print(1)
    print("0 0 2 1 1 -1 1")
elif seed == 2:
    print(10000)
    for _ in range(10000):
        print(*make_case(5000, 10000))
elif seed == 3:
    print(10000)
    for _ in range(10000):
        print(*make_case(2, 30))
else:
    n = rng.randint(1, 3000)
    print(n)
    for _ in range(n):
        print(*make_case(2, 10000))
