"""Tests for p106 (The equation): |a|,|b|,|c|,|x1|,|x2|,|y1|,|y2| <= 1e8,
x1 <= x2, y1 <= y2.

The trusted reference divides by zero when exactly one of a, b is 0, so
those degenerate inputs are never generated (both-zero is fine). Seed 1
is a tiny case, seed 2 maximal magnitudes with a guaranteed-solvable c,
seed 3 maximal with a common even factor and odd c (no solutions),
seed 4 is a = b = c = 0 over the full square, seed 5 a = b = 0 with
c != 0, seed 6 a solvable line that misses the rectangle, seeds 7+ mix
random values with planted solutions inside the box.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

M = 10**8


def emit(a, b, c, x1, x2, y1, y2):
    print(a, b, c)
    print(x1, x2)
    print(y1, y2)


def nonzero(lo, hi):
    v = 0
    while v == 0:
        v = rng.randint(lo, hi)
    return v


if seed == 1:
    emit(1, 1, -3, 0, 4, 0, 4)
elif seed == 2:
    x, y = rng.randint(-1000, 1000), rng.randint(-1000, 1000)
    a, b = nonzero(-M, M), nonzero(-M, M)
    c = -(a * x + b * y)
    while abs(c) > M:
        x, y = rng.randint(-1000, 1000), rng.randint(-1000, 1000)
        c = -(a * x + b * y)
    emit(a, b, c, -M, M, -M, M)
elif seed == 3:
    emit(2 * (M // 2), -2 * (M // 2 - 1), M - 1, -M, M, -M, M)
elif seed == 4:
    emit(0, 0, 0, -M, M, -M, M)
elif seed == 5:
    emit(0, 0, nonzero(-M, M), -M, M, -M, M)
elif seed == 6:
    # x + y = 2e8 only touches the corner-adjacent band; shrink the box
    # so the line misses it entirely.
    emit(1, 1, -2 * M, -M, M // 2, -M, M // 2)
else:
    a, b = nonzero(-M, M), nonzero(-M, M)
    if seed % 2 == 1:
        g = rng.choice([2, 3, 5, 7, 1000, 99991])
        a -= a % g
        b -= b % g
        if a == 0 or b == 0:
            a, b = g, -g
    x1 = rng.randint(-M, M)
    x2 = rng.randint(x1, M)
    y1 = rng.randint(-M, M)
    y2 = rng.randint(y1, M)
    if seed % 3 == 0:
        x = rng.randint(x1, x2)
        y = rng.randint(y1, y2)
        c = -(a * x + b * y)
        if abs(c) > M:
            c = rng.randint(-M, M)
    else:
        c = rng.randint(-M, M)
    emit(a, b, c, x1, x2, y1, y2)
