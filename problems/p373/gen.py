"""Tests for p373 (Carlsson vs. Winnie-the-Pooh): first line "N R"
(1 <= N <= 4, 1 <= R <= 100), then N lines "a b c" with integers in
[-1000, 1000], each line ax+by+c=0 cutting the circle of radius R
centered at the origin in exactly two points (center distance < R).

The two hard axes are the arrangement geometry (up to 11 pieces, high
degree vertices, thin slivers) and the eating minimax, where greedy is
wrong and equal-area ties matter.

Seed 1 is the minimal one-diameter R=1 pizza. Seed 2 is N=4, R=100 in
general position - 11 pieces, the largest game tree. Seed 3 is N=4
concurrent diameters (8 equal sectors meeting in a degree-8 vertex, all
ties). Seed 4 is the statement sample, seed 5 four parallel chords,
seed 6 near-tangent chords slicing off tiny slivers, seed 7 two
perpendicular diameters (4 equal quarters), seed 8 a chord triangle
around the center. Seeds 9+ are random valid line sets.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def valid(a, b, c, r):
    return (a, b) != (0, 0) and c * c < r * r * (a * a + b * b)


def norm_key(a, b, c):
    g = math.gcd(math.gcd(abs(a), abs(b)), abs(c))
    a, b, c = a // g, b // g, c // g
    if (a, b, c) < (0, 0, 0) or (a < 0 or (a == 0 and b < 0)):
        a, b, c = -a, -b, -c
    return (a, b, c)


def emit(r, lines):
    for a, b, c in lines:
        assert valid(a, b, c, r), (a, b, c, r)
    print(len(lines), r)
    for a, b, c in lines:
        print(a, b, c)


def random_line(r, near_tangent=False):
    while True:
        a = rng.randint(-1000, 1000)
        b = rng.randint(-1000, 1000)
        if (a, b) == (0, 0):
            continue
        cmax = min(1000, int(r * math.hypot(a, b)))
        while cmax > 0 and not valid(a, b, cmax, r):
            cmax -= 1
        if near_tangent:
            if cmax < 3:
                continue
            c = rng.choice([-1, 1]) * rng.randint(cmax - 2, cmax)
        else:
            c = rng.randint(-cmax, cmax)
        if valid(a, b, c, r):
            return (a, b, c)


def random_set(r, n, near_tangent_prob=0.0):
    lines = []
    keys = set()
    while len(lines) < n:
        ln = random_line(r, rng.random() < near_tangent_prob)
        k = norm_key(*ln)
        if k in keys:
            continue
        keys.add(k)
        lines.append(ln)
    return lines


if seed == 1:
    emit(1, [(1, 0, 0)])
elif seed == 2:
    emit(100, [(13, 7, -150), (-5, 17, 220), (11, -9, 75), (2, 23, -310)])
elif seed == 3:
    emit(100, [(1, 0, 0), (0, 1, 0), (1, 1, 0), (1, -1, 0)])
elif seed == 4:
    emit(100, [(1, 0, 0)])  # statement sample
elif seed == 5:
    emit(100, [(3, 4, -400), (3, 4, -150), (3, 4, 100), (3, 4, 350)])
elif seed == 6:
    emit(100, random_set(100, 4, near_tangent_prob=1.0))
elif seed == 7:
    emit(100, [(1, 0, 0), (0, 1, 0)])
elif seed == 8:
    emit(100, [(0, 1, -50), (7, -4, -300), (-7, -4, 300)])
else:
    r = rng.choice([1, 2, rng.randint(3, 99), 100])
    n = rng.randint(1, 4)
    emit(r, random_set(r, n, near_tangent_prob=0.25))
