"""Tests for p266 (Berlion): sphere R in [1,10], two integer scanner points.

|coord| <= 100 and distance from the center at least R + 0.01, which for
integer coordinates means x^2 + y^2 + z^2 >= R^2 + 1. The input is always
tiny, so "maximal" seeds use the extreme values (R = 10, coords +-100).
Adversarial seeds cover identical scanners, antipodal scanners, nested
caps (one scanner far on the same axis) and barely-off-surface scanners.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def rand_point(R, max_c=100):
    while True:
        x = rng.randint(-max_c, max_c)
        y = rng.randint(-max_c, max_c)
        z = rng.randint(-max_c, max_c)
        if x * x + y * y + z * z >= R * R + 1:
            return (x, y, z)


def near_surface_point(R):
    # Smallest integer points just above the surface, e.g. (R, 1, 0).
    candidates = [(R, 1, 0), (R, 0, 1), (R, 1, 1), (0, R, 1), (1, 1, R)]
    return rng.choice(candidates)


def emit(R, a, b):
    print(R)
    print(*a)
    print(*b)


if seed == 1:
    emit(1, (1, 1, 0), (0, 1, 1))
elif seed == 2:
    emit(10, (100, 100, 100), (-100, -100, -100))
elif seed == 3:
    emit(10, (100, -100, 100), (100, -100, 100))
elif seed == 4:
    R = rng.randint(1, 10)
    a = near_surface_point(R)
    emit(R, a, (a[0], -a[1], a[2]))
elif seed == 5:
    R = rng.randint(1, 10)
    a = near_surface_point(R)
    # Nested caps: second scanner far away along (almost) the same axis.
    emit(R, a, (a[0] * 9, a[1] * 9, a[2] * 9))
elif seed == 6:
    R = 10
    emit(R, (0, 0, 11), (0, 0, -11))
else:
    R = rng.randint(1, 10)
    emit(R, rand_point(R), rand_point(R))
